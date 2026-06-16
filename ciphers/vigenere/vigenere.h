#pragma once

#include <string>
#include <iostream>
#include <random>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>

#define LAT_START 97
#define LAT_END 122

#define LAT_UP_START 65
#define LAT_UP_END 90

#define RU_START 1072
#define RU_END 1105

#define RU_UP_START 1040
#define RU_UP_END 1071

#define SYMBOLS_START 0
#define SYMBOLS_END 64

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

class EXPORT Vigenere{
    public:
    std::wstring EncryptText(const std::wstring& text, const std::wstring& key);
    std::wstring DecryptText(const std::wstring& encrypted, const std::wstring& key);
    
    void EncryptTextFile(const std::string& path, const std::string& out_path, const std::wstring& key);
    void DecryptTextFile(const std::string& path, const std::string& out_path, const std::wstring& key);

    void EncryptBinary(const std::string& path, const std::string& out_path, const std::string& key);
    void DecryptBinary(const std::string& path, const std::string& out_path, const std::string& key);

    std::wstring KeyGenerator(size_t length, bool isLatin);
    void ByteFileKeyGenerator(std::string& path, size_t length);

    private:
    wchar_t Encrypt(const std::wint_t& c, const std::wint_t& key);
    wchar_t Decrypt(const std::wint_t& c, const std::wint_t& key);
};
