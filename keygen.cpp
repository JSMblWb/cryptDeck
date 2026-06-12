#include "keygen.h"

//================================================= текстовый ключ
std::wstring KEYGEN::textKey(size_t length, bool isLatin){
    const std::wstring latinCharset = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::wstring ruCharset = L"0123456789АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзиклмнопрстуфхцчшщъыьэюя";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution;
    std::wstring key;

        if (length == 0)
                throw std::runtime_error("Длина генерируемого ключа не может быть равна нулю");

    if (isLatin)
        distribution = std::uniform_int_distribution<>(0, latinCharset.size()-1);
    else
        distribution = std::uniform_int_distribution<>(0, ruCharset.size()-1);

    key.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        key += (isLatin) ? latinCharset[distribution(generator)] : ruCharset[distribution(generator)];
    }

    return key;
}
//================================================= байтник
void KEYGEN::byteFileKey(std::string& path, size_t length){
    if (length == 0)
        throw std::runtime_error("Длина ключа не может быть равна нулю");

    std::ofstream out(path, std::ios::binary);

    if (!out.is_open())
        throw std::runtime_error("Не удалось открыть файл для записи ключа");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dist(0, 255);
    std::vector<unsigned char> buffer(BUFFER_SIZE);

    for (size_t remaining = length; remaining > 0; ){
        size_t chunk = std::min(remaining, (size_t) BUFFER_SIZE);

        for (size_t i = 0; i < chunk; i++){
            buffer[i] = dist(gen);
        }

        out.write(reinterpret_cast<const char*>(buffer.data()), chunk);
        remaining -= chunk;
    }

    std::cout << "Файл с ключом находится по адресу: " << path << std::endl;

    out.close();
}
//================================================= 32 хекс
int KEYGEN::hex32Key() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);

	std::ofstream file("hex32Key");
    
	for (int i = 0; i < 100; i++) {
		file << std::hex << std::setw(8) << std::setfill('0') << dis(gen) << "\n";
	}

	file.close();

	return 0;
}
