#include "vigenere.h"

std::wstring Vigenere::EncryptText(const std::wstring& text, const std::wstring& key){
    std::wstring out = std::wstring(text.size(), 0);

    for (int i = 0; i < text.size(); i++){
        out[i] = Encrypt(text[i], key[i % key.size()]);
    }

    return out;
}

std::wstring Vigenere::DecryptText(const std::wstring& encrypted, const std::wstring& key){
    std::wstring out = std::wstring(encrypted.size(), 0);
    
    for (int i = 0; i < encrypted.size(); i++){
        out[i] = Decrypt(encrypted[i], key[i % key.size()]);
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

void Vigenere::EncryptTextFile(const std::string& path, const std::wstring& key){
    std::wifstream in(path, std::ios::binary);;
    std::wofstream out("temp.tmp", std::ios::binary);
    wchar_t c;
    size_t i = 0;

    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    while (in.get(c)){
        std::streampos pos = in.tellg();

        out.put(Encrypt(c, key[i % key.size()]));

        i++;
    }

    in.close();
    out.close();

    std::filesystem::rename("temp.tmp", path);
}

void Vigenere::DecryptTextFile(const std::string& path, const std::wstring& key){
    std::wifstream in(path, std::ios::binary);;
    std::wofstream out("temp.tmp", std::ios::binary);
    wchar_t c;
    size_t i = 0;

    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    while (in.get(c)){
        std::streampos pos = in.tellg();

        out.put(Decrypt(c, key[i % key.size()]));

        i++;
    }

    in.close();
    out.close();

    std::filesystem::rename("temp.tmp", path);
}

void Vigenere::EncryptBinary(const std::string& path, const std::string& key){
    std::ifstream in(path, std::ios::binary);;
    std::ofstream out("temp.tmp", std::ios::binary);
    size_t i = 0;

    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    unsigned char byte;

    while(in.read(reinterpret_cast<char*>(&byte), 1)){
        unsigned char keyByte = static_cast<unsigned char>(key[i % key.size()]);

        byte = (byte + keyByte) % 256;
        out.write(reinterpret_cast<const char*>(&byte), 1);
        i++;
    }

    in.close();
    out.close();

    std::filesystem::rename("temp.tmp", path);
}

void Vigenere::DecryptBinary(const std::string& path, const std::string& key){
    std::ifstream in(path, std::ios::binary);;
    std::ofstream out("temp.tmp", std::ios::binary);
    size_t i = 0;

    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    unsigned char byte;

    while(in.read(reinterpret_cast<char*>(&byte), 1)){
        unsigned char keyByte = static_cast<unsigned char>(key[i % key.size()]);

        byte = (byte - keyByte + 256) % 256;

        out.write(reinterpret_cast<const char*>(&byte), 1);
        i++;
    }

    in.close();
    out.close();

    std::filesystem::rename("temp.tmp", path);
}

wchar_t Vigenere::Encrypt(const std::wint_t& c, const std::wint_t& key){
    wint_t letter = c;
    wint_t keyAlphLength = (key >= RU_START) ? RU_START : ((key >= RU_UP_START) ? RU_UP_START : ((key >= LAT_START) ? LAT_START : ((key >= LAT_UP_START) ? LAT_UP_START : SYMBOLS_START)));
    int charType = (letter >= RU_START) ? 0 : ((letter >= RU_UP_START) ? 1 : ((letter >= LAT_START) ? 2 : ((letter >= LAT_UP_START) ? 3 : 4)));
    wint_t result = letter + (key - keyAlphLength);

    if (key == 1025)
        result = letter + (key - RU_UP_START);
    if (letter == 1025)
        charType = 1;

    switch (charType){
        case 0: return (result > RU_END) ? (result - (RU_END-RU_START+1)) : result;
        case 1: return (result > RU_UP_END) ? (result - (RU_UP_END-RU_UP_START+1)) : result;
        case 2: return (result > LAT_END) ? (result - (LAT_END-LAT_START+1)) : result;
        case 3: return (result > LAT_UP_END) ? (result - (LAT_UP_END-LAT_UP_START+1)) : result;
        case 4: return (result > SYMBOLS_END) ? (result - (SYMBOLS_END-SYMBOLS_START+1)) : result;
        default: return result;
    }
}

wchar_t Vigenere::Decrypt(const std::wint_t& c, const std::wint_t& key){
    wint_t letter = c;
    wint_t keyAlphLength = (key >= RU_START) ? RU_START : ((key >= RU_UP_START) ? RU_UP_START : ((key >= LAT_START) ? LAT_START : ((key >= LAT_UP_START) ? LAT_UP_START : SYMBOLS_START)));
    int charType = (letter >= RU_START) ? 0 : ((letter >= RU_UP_START) ? 1 : ((letter >= LAT_START) ? 2 : ((letter >= LAT_UP_START) ? 3 : 4)));
    wint_t result = letter - (key - keyAlphLength);
    
    if (key == 1025)
        result = letter - (key - RU_UP_START);
    if (letter == 1025)
        charType = 1;

    switch (charType){
        case 0: return (result < RU_START) ? (result + (RU_END-RU_START+1)) : result;
        case 1: return (result < RU_UP_START) ? (result + (RU_UP_END-RU_UP_START+1)) : result;
        case 2: return (result < LAT_START) ? (result + (LAT_END-LAT_START+1)) : result;
        case 3: return (result < LAT_UP_START) ? (result + (LAT_UP_END-LAT_UP_START+1)) : result;
        case 4: return (result < SYMBOLS_START) ? (result + (SYMBOLS_END-SYMBOLS_START+1)) : result;
        default: return result;
    }
}