#pragma once

#include <string>
#include <iostream>
#include <random>

#ifdef _WIN32
    #ifdef BUILD
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

class EXPORT Vigenere{
    public:
    std::wstring Encrypt(const std::wstring& text, const std::wstring& key);
    std::wstring Decrypt(const std::wstring& encrypted, const std::wstring& key);
    std::wstring KeyGenerator(size_t length, bool isLatin);
};