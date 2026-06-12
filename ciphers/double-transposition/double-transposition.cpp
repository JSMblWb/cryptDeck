#include <double-transposition.h>

//================================================= порядок столбцов для ключа
std::vector<int> getColumnOrder(const std::string& key) {
	std::vector<std::pair<char, int>> indexed;
	for (int i = 0; i < key.size(); ++i) {
		indexed.emplace_back(key[i], i);
	}
	std::sort(indexed.begin(), indexed.end());
	std::vector<int> order;
	for (const pair<char, int> p : indexed) {
		order.push_back(p.second);
	}
	return order;
	}
//================================================= одиночный шифр
std::string singleEncrypt(const std::string& text, const std::string& key) {
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
std::string singleDecrypt(const std::string& cipher, const std::string& key) {
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
//================================================== двойной шифр
void doubleEncrypt(const std::string& inputPath, const std::string& outputPath) {
	std::string binaryNotEncrypted = loadFile(inputPath);
	std::string binaryEncryptedOnce = singleEncrypt(binaryNotEncrypted, key1);
	exportFile(singleEncrypt(binaryEncryptedOnce, key2), outputPath);
}

//================================================= двойной дешифр
void doubleDecrypt(const std::string& inputPath, const std::string outputPath) {
	std::string binaryNotDecrypted = loadFile(inputPath);
	std::string binaryDecryptedOnce = singleDecrypt(binaryNotEncrypted, key2);
	exportFile(singleDecrypt(binaryDecryptedOnce, key1), /*путь*/);
}
//================================================= кейген

//================================================= читалка путей
std::string loadFile(const std::string& filePath) {
	//открываем поток в бинарном инпут режиме
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	//узнаём размер файла
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	//переводим в вектор
	std::string buffer(size);
	if (file.read(buffer.data(), size)) {
		return buffer;
	}
}
//================================================= заполнялка по путям
void exportFile(const std::string& binaryData, const std::filesystem::path& destination) {
	//открываем поток в бинарном аутпут режиме
	std::ofstream outFile(destination, std::ios::out);

	//записываем файлы и их длину
	outFile.write(binaryData.data(), binaryData.size());
	
	outFile.close();

}
