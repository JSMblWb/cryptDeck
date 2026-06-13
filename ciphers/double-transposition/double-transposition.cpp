include "double-transposition.h"

void exportFile(const std::string& binaryData, const std::filesystem::path& destination);
std::string loadFile(const std::string& path);

//================================================= порядок столбцов для ключа
std::vector<int> DT::getColumnOrder(const std::string& key) {
	std::vector<std::pair<char, int>> indexed;
	for (int i = 0; i < key.size(); ++i) {
		indexed.emplace_back(key[i], i);
	}
	std::sort(indexed.begin(), indexed.end());
	std::vector<int> order;
	for (const std::pair<char, int> p : indexed) {
		order.push_back(p.second);
	}
	return order;
	}
//================================================= одиночный шифр
std::string DT::singleEncrypt(const std::string& text, const std::string& key) {
	int k = key.size();
	if (k == 0 || text.empty()) return text;

	int len = text.size();
	int rows = (len + k - 1) / k; // ceil(len / k)
	int longCols = (len % k == 0) ? k : len % k; //число столбцов, имеющих rows символов

	//заполняем таблицу построчно
	std::vector<std::string> grid(rows, std::string(k, '\0'));
	for (int i = 0; i < len; ++i) {
		int r = i / k;
		int c = i % k;
		grid[r][c] = text[i];
	}

	std::vector<int> colOrder = getColumnOrder(key);
	std::string cipher;
	for (int colIdx : colOrder) {
		int colLen = (colIdx < longCols) ? rows : rows - 1;
		for (int r = 0; r < colLen; ++r) {
			cipher.push_back(grid[r][colIdx]);
		}
	}
	return cipher;
}
//================================================= одиночный дешифр
std::string DT::singleDecrypt(const std::string& cipher, const std::string& key) {
	int k = key.size();
	if (k == 0 || cipher.empty()) return cipher;

	int len = cipher.size();
	int rows = (len + k - 1) / k;
	int longCols = (len % k == 0) ? k : len % k;

	std::vector<int> colOrder = getColumnOrder(key);
	std::vector<std::string> columns(k);
	int pos = 0;

	//заполняем столбцы в порядке, заданном ключом
	for (int sortedPos = 0; sortedPos < k; ++sortedPos) {
		int origCol = colOrder[sortedPos];
		int colLen = (origCol < longCols) ? rows : rows - 1;
		columns[origCol] = cipher.substr(pos, colLen);
		pos += colLen;
	}

	//читаем таблицу построчно
	std::string plain;
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < k; ++c) {
			if (r < columns[c].size()) {
				plain.push_back(columns[c][r]);
			}
		}
	}
	return plain;
}
//================================================== двойной шифр бинарник
void DT::doubleEncrypt(const std::string& inputPath, const std::string& outputPath, const std::string& key1Path, const std::string& key2Path) {
	std::string key1 = loadFile(key1Path), key2 = loadFile(key2Path);
	std::string binaryNotEncrypted = loadFile(inputPath);
	std::string binaryEncryptedOnce = singleEncrypt(binaryNotEncrypted, key1);
	exportFile(singleEncrypt(binaryEncryptedOnce, key2), outputPath);
}

//================================================= двойной дешифр бинарник
void DT::doubleDecrypt(const std::string& inputPath, const std::string& outputPath, const std::string& key1Path, const std::string& key2Path) {
	std::string key1 = loadFile(key1Path), key2 = loadFile(key2Path);
	std::string binaryNotDecrypted = loadFile(inputPath);
	std::string binaryDecryptedOnce = singleDecrypt(binaryNotDecrypted, key2);
	exportFile(singleDecrypt(binaryDecryptedOnce, key1), outputPath);
}

//================================================= двойной шифр текст
std::string DT::doubleEncryptText(const std::string& text, const std::string& key1, const std::string& key2){
	std::string textEncryptedOnce = singleEncrypt(text, key1);
	std::string textEncryptedTwice = singleEncrypt(textEncryptedOnce, key2);

	return textEncryptedTwice;
}

//================================================= двойной дешифр текст
std::string DT::doubleDecryptText(const std::string& text, const std::string& key1, const std::string& key2){
	std::string textDecryptedOnce = singleDecrypt(text, key2);
	std::string textDecryptedTwice = singleDecrypt(textDecryptedOnce, key1);

	return textDecryptedTwice;
}

//================================================= читалка путей
std::string DT::loadFile(const std::string& filePath) {
	//открываем поток в бинарном инпут режиме
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if (!file.is_open())
		throw std::runtime_error("Ошибка открытия файла");

	//узнаём размер файла
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	//переводим в строку
	std::string buffer(size, '/0');
	if (file.read(buffer.data(), size)) {
		return buffer;
	} else {
		throw std::runtime_error("Ошибка чтения файла");
	}
}

//================================================= заполнялка по путям
void DT::exportFile(const std::string& binaryData, const std::filesystem::path& destination) {
	//открываем поток в бинарном аутпут режиме
	std::ofstream outFile(destination, std::ios::out);

	if (!outFile.is_open())
		throw std::runtime_error("Ошибка открытия файла");

	//записываем файлы и их длину
	outFile.write(binaryData.data(), binaryData.size());
	
	outFile.close();

}
