#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <filesystem>

#define BUFFER_SIZE 4096

#ifdef _WIN32
	#ifdef BUILD
		#define EXPORT __declspec(dllexport)
	#else
		#define EXPORT __declspec(dllimport)
	#endif
#else
	#define EXPORT __attribute__((visibility("default")))
#endif

class EXPORT DT {
	public:
	std::vector<int> getColumnOrder(const std::string& key);
	std::string singleEncrypt(const std::string& text, const std::string& key);
	std::string singleDecrypt(const std::string& cipher, const std::string& key);
	std::string loadFile(const std::string& filePath);
	void exportFile(const std::string& binaryData, const std::filesystem::path& destination);

	void doubleEncrypt(const std::string& inputPath, const std::string& outputPath, const std::string& key1, const std::string& key2);
	void doubleDecrypt(const std::string& inputPath, const std::string& outputPath, const std::string& key1, const std::string& key2);

	std::string doubleEncryptText(const std::string& text, const std::string& key1, const std::string& key2);
	std::string doubleDecryptText(const std::string& text, const std::string& key1, const std::string& key2);
};
