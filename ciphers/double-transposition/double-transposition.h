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
	void doubleEncrypt(const std::string& inputPath, const std::string& outputPath);
	void doubleDecrypt(const std::string& inputPath, const std::string& outputPath);
}
