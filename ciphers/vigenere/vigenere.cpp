#include "vigenere.h"

#define LAT_START 97
#define LAT_END 122
#define RU_START 1072
#define RU_END 1105
#define SYMBOLS_START 32
#define SYMBOLS_END 64

std::wstring Vigenere::Encrypt(const std::wstring& text, const std::wstring& key){
    std::wstring out = std::wstring(text.size(), 0);

    for (int i = 0; i < text.size(); i++){
        wint_t letter = towlower(text[i]);
        wint_t keyAlphLength = (towlower(key[i % key.size()]) >= RU_START) ? RU_START : ((towlower(key[i % key.size()]) >= LAT_START) ? LAT_START : SYMBOLS_START);
        int charType = (letter >= RU_START) ? 0 : ((letter >= LAT_START) ? 1 : 2);
        wint_t result = letter + (towlower(key[i % key.size()]) - keyAlphLength);

        switch (charType){
            case 0: out[i] = (result > RU_END) ? (result - (RU_END-RU_START)) : result; break;
            case 1: out[i] = (result > LAT_END) ? (result - (LAT_END-LAT_START)) : result; break;
            case 2: out[i] = (result > SYMBOLS_END) ? (result - (SYMBOLS_END-SYMBOLS_START)) : result; break;
            default: out[i] = result; break;
        }
    }

    return out;
}

std::wstring Vigenere::Decrypt(const std::wstring& encrypted, const std::wstring& key){
    std::wstring out = std::wstring(encrypted.size(), 0);
    
    for (int i = 0; i < encrypted.size(); i++){
        wint_t letter = towlower(encrypted[i]);
        wint_t keyAlphLength = (towlower(key[i % key.size()]) >= RU_START) ? RU_START : ((towlower(key[i % key.size()]) >= LAT_START) ? LAT_START : SYMBOLS_START);
        int charType = (letter >= RU_START) ? 0 : ((letter >= LAT_START) ? 1 : 2);
        wint_t result = letter - (towlower(key[i % key.size()]) - keyAlphLength);

        switch (charType){
            case 0: out[i] = (result < RU_START) ? (result + (RU_END-RU_START)) : result; break;
            case 1: out[i] = (result < LAT_START) ? (result + (LAT_END-LAT_START)) : result; break;
            case 2: out[i] = (result < SYMBOLS_START) ? (result + (SYMBOLS_END-SYMBOLS_START)) : result; break;
            default: out[i] = result; break;
        }
    }

    return out;
}

std::wstring Vigenere::KeyGenerator(size_t length, bool isLatin){
    const std::wstring latinCharset = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::wstring ruCharset = L"0123456789АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзиклмнопрстуфхцчшщъыьэюя";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution;
    std::wstring key;

    if (isLatin)
        distribution = std::uniform_int_distribution<>(0, latinCharset.size()-1);
    else
        distribution = std::uniform_int_distribution<>(0, ruCharset.size()-1);

    key.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        key += (isLatin) ? latinCharset[distribution(generator)] : ruCharset[distribution(generator)];
    }

    return key;
}