#pragma once

#include <string>
#include <stdexcept>
#include <random>
#include <fstream>
#include <filesystem>
#include <iostream>

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

class EXPORT KEYGEN{
    public:
    std::wstring textKey(size_t length, bool isLatin);
    void byteFileKey(std::string& path, size_t length);
};

