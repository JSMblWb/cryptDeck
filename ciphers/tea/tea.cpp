#include "tea.h"

//================================================= базовые операции над одним блоком 64 бита = 2 x uint32_t
void encryptBlock(uint32_t& v0, uint32_t& v1, const uint32_t key[4]) {
	const uint32_t delta = 0x9E3779B9;
	uint32_t sum = 0;
	for (int i = 0; i < 32; ++i) {
		sum += delta;
		v0 += ((v1 << 4) + key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + key[1]);
		v1 += ((v0 << 4) + key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + key[3]);
	}
}

void decryptBlock(uint32_t& v0, uint32_t& v1, const uint32_t key[4]) {
	const uint32_t delta = 0x9E3779B9;
	uint32_t sum = delta * 32; //0xC6EF3720
	for (int i = 0; i < 32; ++i) {
		v1 -= ((v0 << 4) + key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + key[3]);
		v0 -= ((v1 << 4) + key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + key[1]);
		sum -= delta;
	}
}
//================================================= конвертация hex-строки в 128-битный ключ 4 x uint32_t, little-endian порядок байт
bool parseKey(const std::string& hex, uint32_t key[4]) {
	if (hex.length() != 32*8) return false;
	for (int i = 0; i < 4; ++i) {
		std::string part = hex.substr(i * 8, 8);
		try {
			key[i] = std::stoul(part, nullptr, 16);
		} catch (...) {
			return false;
		}
	}
	return true;
}
//================================================= дополнение PKCS#7
std::vector<uint8_t> pkcs7Pad(const std::vector<uint8_t>& data, size_t blockSize = 8) {
	size_t padLen = blockSize - (data.size() % blockSize);
	std::vector<uint8_t> padded = data;
	padded.insert(padded.end(), padLen, static_cast<uint8_t>(padLen));
	return padded;
}
//================================================= снятие дополнения PKCS#7
std::vector<uint8_t> pkcs7Unpad(const std::vector<uint8_t>& data) {
	if (data.empty()) throw std::runtime_error("Дополнение отсутсвует");
	uint8_t padLen = data.back();
	if (padLen == 0 || padLen > 8) throw std::runtime_error("Дополнение некорректно");
	for (size_t i = data.size() - padLen; i < data.size(); ++i) {
		if (data[i] != padLen) throw std::runtime_error("Дополнение некорректно");
	}
	return std::vector<uint8_t>(data.begin(), data.end() - padLen);
}
//================================================= шифрование в режиме CBC
std::vector<uint8_t> cbcEncrypt(const std::vector<uint8_t>& plain, const uint32_t key[4]) {
	//генерация случайного IV
	std::vector<uint8_t> iv(8);
	std::random_device rd;
	std::mt19937_64 gen(rd());
	uint64_t random_iv = gen(); //64 случайных бита
	std::memcpy(iv.data(), &random_iv, 8);

	//дополнение открытого текста
	std::vector<uint8_t> padded = pkcs7Pad(plain, 8);
	std::vector<uint8_t> cipher(padded.size() + 8); //место для IV
	std::copy(iv.begin(), iv.end(), cipher.begin());

	uint32_t prev0, prev1;
	std::memcpy(&prev0, iv.data(), 4);
	std::memcpy(&prev1, iv.data() + 4, 4);

	for (size_t i = 0; i < padded.size(); i += 8) {
		uint32_t v0, v1;
		std::memcpy(&v0, &padded[i], 4);
		std::memcpy(&v1, &padded[i + 4], 4);

		//xor с предыдущим шифротекстом (или IV)
		v0 ^= prev0;
		v1 ^= prev1;

		encryptBlock(v0, v1, key);

		//сохраняем шифротекст
		std::memcpy(&cipher[8 + i], &v0, 4);
		std::memcpy(&cipher[8 + i + 4], &v1, 4);

		prev0 = v0;
		prev1 = v1;
	}
	return cipher;
}
//================================================= расшифрование в режиме CBC
std::vector<uint8_t> cbcDecrypt(const std::vector<uint8_t>& cipher, const uint32_t key[4]) {
	if (cipher.size() < 16) throw std::runtime_error("Шифротекст слишком короткий (минимум 16 байт)");
	if (cipher.size() % 8 != 0) throw std::runtime_error("Длина шифротекста должна быть кратна 8");

	//извлечение IV
	uint32_t prev0, prev1;
	std::memcpy(&prev0, cipher.data(), 4);
	std::memcpy(&prev1, cipher.data() + 4, 4);

	std::vector<uint8_t> padded(cipher.size() - 8);
	for (size_t i = 8; i < cipher.size(); i += 8) {
		uint32_t v0, v1;
		std::memcpy(&v0, &cipher[i], 4);
		std::memcpy(&v1, &cipher[i + 4], 4);

		uint32_t dec0 = v0, dec1 = v1;
		decryptBlock(dec0, dec1, key);

		//xor с предыдущим шифротекстом (или IV)
		dec0 ^= prev0;
		dec1 ^= prev1;

		std::memcpy(&padded[i - 8], &dec0, 4);
		std::memcpy(&padded[i - 8 + 4], &dec1, 4);

		prev0 = v0;
		prev1 = v1;
	}
	
	try {
		std::vector<uint8_t> unpadded = pkcs7Unpad(padded);

		return unpadded;
	} catch (const std::runtime_error &e) {
		std::cout << "Ошибка: " << e.what() << std::endl;
	}

	return padded;
}
//================================================= читалка файлов
std::vector<uint8_t> loadFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
//================================================= записывалка файлов
void exportFile(const std::string& filename, const std::vector<uint8_t>& data) {
	std::ofstream file(filename, std::ios::binary);
	file.write(reinterpret_cast<const char*>(data.data()), data.size());
}
//================================================= основная функция
int TEA::cipherFunc(int mode, std::string keyHex, std::string inFile, std::string outFile) {
	uint32_t key[4];
	if (!parseKey(keyHex, key)) {
		throw std::runtime_error("Некорректный ключ. Используйте 32 hex символа (128 бит).");
	}

	try {
		std::vector<uint8_t> input = loadFile(inFile);

		if (mode == 0) {
			std::vector<uint8_t> output = cbcEncrypt(input, key);
			exportFile(outFile, output);
			std::cout << "Шифровка завершена.\n";
		} else if (mode == 1) {
			std::vector<uint8_t> output = cbcDecrypt(input, key);
			exportFile(outFile, output);
			std::cout << "Расшифровка завершена.\n";
		}
	} catch (const std::runtime_error &e) {
		std::cerr << "Ошибка: " << e.what() << std::endl;
	}

	return 0;
}

std::string TEA::cipherFuncText(int mode, std::string keyHex, std::string text){
	uint32_t key[4];

	if (!parseKey(keyHex, key)) {
		throw std::runtime_error("Некорректный ключ. Используйте 32 hex символа (128 бит)");
	}

	try {
		std::vector<uint8_t> input(text.begin(), text.end());

		if (mode == 0){
			std::vector<uint8_t> output = cbcEncrypt(input, key);
			return std::string(output.begin(), output.end());
		} else if (mode == 1){
			std::vector<uint8_t> output = cbcDecrypt(input, key);
			return std::string(output.begin(), output.end());
		}
	} catch (const std::runtime_error &e){
		std::cerr << "Ошибка: " << e.what() << std::endl;
	}
}
