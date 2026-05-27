#include "vernam.h"

std::wstring Vernam::Encrypt(const std::wstring& text, const std::wstring& key){
    std::wstring out = std::wstring(text.size(), 0);

    if (key.size() < text.size())
        throw std::runtime_error("Длина ключа меньше длины текста");

    for (int i = 0; i < text.size(); i++){
        out[i] = text[i] ^ key[i];
    }

    return out;
}

std::wstring Vernam::Decrypt(const std::wstring& encrypted, const std::wstring& key){
    std::wstring out = std::wstring(encrypted.size(), 0);

    if (key.size() < encrypted.size())
        throw std::runtime_error("Длина ключа меньше длины шифротекста");

    for (int i = 0; i < encrypted.size(); i++){
        out[i] = encrypted[i] ^ key[i];
    }

    return out;
}

std::wstring Vernam::KeyGenerator(size_t length, bool isLatin){
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