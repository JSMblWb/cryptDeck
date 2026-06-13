#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <random>
#include <stdexcept>

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

class EXPORT TEA {
	public:
	int cipherFunc(int mode, const std::string& keyHex, const std::string& inFile, const std::string& outFile);
	std::string cipherFuncText(int mode, const std::string& keyHex, const std::string& text);
	void encryptBlock(uint32_t& v0, uint32_t& v1, const uint32_t key[4]);
	void decryptBlock(uint32_t& v0, uint32_t& v1, const uint32_t key[4]);
	bool parseKey(const std::string& hex, uint32_t key[4]);
	std::vector<uint8_t> pkcs7Pad(const std::vector<uint8_t>& data, size_t blockSize);
	std::vector<uint8_t> pkcs7Unpad(const std::vector<uint8_t>& data);
	std::vector<uint8_t> cbcEncrypt(const std::vector<uint8_t>& plain, const uint32_t key[4]);
	std::vector<uint8_t> cbcDecrypt(const std::vector<uint8_t>& cipher, const uint32_t key[4]);
	std::vector<uint8_t> loadFile(const std::string& filename);
	void exportFile(const std::string& filename, const std::vector<uint8_t>& data);

};
