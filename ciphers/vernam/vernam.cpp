#include "vernam.h"

std::wstring Vernam::EncryptOrDecryptText(const std::wstring& text, const std::wstring& key){
    std::wstring out = std::wstring(text.size(), 0);

    if (key.size() < text.size())
        throw std::runtime_error("Длина ключа меньше длины текста");

    for (int i = 0; i < text.size(); i++){
        out[i] = CipherFunc(text[i], key[i]);
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

void Vernam::EncryptOrDecryptTextFile(const std::string& path, const std::wstring& key){
    std::wifstream in(path, std::ios::binary);;
    std::wofstream out("temp.tmp", std::ios::binary);
    wchar_t c;
    size_t i = 0;

    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    while (in.get(c)){
        std::streampos pos = in.tellg();

        out.put(CipherFunc(c, key[i % key.size()]));

        i++;
    }

    in.close();
    out.close();

    std::filesystem::rename("temp.tmp", path);
}

void Vernam::EncryptOrDecryptBinary(const std::string& path, const std::string& key){
    std::ifstream in(path, std::ios::binary);;
    std::ofstream out("temp.tmp", std::ios::binary);
    size_t i = 0;

    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    unsigned char byte;

    while(in.read(reinterpret_cast<char*>(&byte), 1)){
        byte ^= key[i % key.size()];
        out.write(reinterpret_cast<const char*>(&byte), 1);
        i++;
    }

    in.close();
    out.close();

    std::filesystem::rename("temp.tmp", path);
}

std::wint_t Vernam::CipherFunc(const std::wint_t& c, const std::wint_t& key){
    return c ^ key;
}