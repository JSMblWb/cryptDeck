#pragma once

#include <string>
#include <stdexcept>
#include <random>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
    #ifdef BUILD
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#else
    #define EXPORT __attribute__((visibility("default")))
#endif

class EXPORT Vernam{
    public:
    std::wstring EncryptOrDecryptText(const std::wstring& text, const std::wstring& key);
    void EncryptOrDecryptTextFile(const std::string& path, const std::wstring& key);
    void EncryptOrDecryptBinary(const std::string& path, const std::string& key);

    std::wstring KeyGenerator(size_t length, bool isLatin);
    
    private:
    std::wint_t CipherFunc(const std::wint_t& c, const std::wint_t& key);
};