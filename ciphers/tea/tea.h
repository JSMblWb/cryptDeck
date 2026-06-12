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
	int cipherFunc(int mode, std::string keyHex, std::string inFile, std::string outFile);
	std::string cipherFuncText(int mode, std::string keyHex, std::string text);
};
