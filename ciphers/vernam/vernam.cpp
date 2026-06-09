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

std::wstring Vernam::TextKeyGenerator(size_t length, bool isLatin){
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

void Vernam::ByteFileKeyGenerator(std::string& path, size_t length){
    if (length == 0)
        throw std::runtime_error("Длина ключа должна быть больше 0");

    std::ofstream out(path, std::ios::binary);

    if (!out.is_open())
        throw std::runtime_error("Не удалось открыть файл для записи ключа");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dist(0, 255);
    std::vector<unsigned char> buffer(BUFFER_SIZE);

    for (size_t remaining = length; remaining > 0; ){
        size_t chunk = std::min(remaining, (size_t) BUFFER_SIZE);

        for (size_t i = 0; i < chunk; i++){
            buffer[i] = dist(gen);
        }

        out.write(reinterpret_cast<const char*>(buffer.data()), chunk);
        remaining -= chunk;
    }

    std::cout << "Файл с ключом находится по адресу: " << path << std::endl;

    out.close();
}

void Vernam::EncryptOrDecryptTextFile(const std::string& path, const std::string& out_path, const std::wstring& key){
    std::wifstream in(path, std::ios::binary);;
    std::wofstream out(out_path, std::ios::binary);
    wchar_t c;
    size_t i = 0;
    size_t fileSize;

    in.seekg(0, std::ios::end);
    fileSize = in.tellg();
    in.seekg(0, std::ios::beg);

    if (key.size() < (fileSize / sizeof(wchar_t)))
        throw std::runtime_error("Длина ключа меньше длины текста в файле");

    if (!in.is_open() || !out.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    while (in.get(c)){
        std::streampos pos = in.tellg();

        out.put(CipherFunc(c, key[i % key.size()]));

        i++;
    }

    in.close();
    out.close();
}

void Vernam::EncryptOrDecryptBinary(const std::string& path, const std::string& out_path, const std::string& key_path){
    std::ifstream key(key_path, std::ios::binary);
    std::ifstream in(path, std::ios::binary);;
    std::ofstream out(out_path, std::ios::binary);
    size_t i = 0;

    if (!in.is_open() || !key.is_open() || !out.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    unsigned char byte, keyByte;

    while(in.read(reinterpret_cast<char*>(&byte), 1) && key.read(reinterpret_cast<char*>(&keyByte), 1)){
        byte ^= keyByte;
        out.write(reinterpret_cast<const char*>(&byte), 1);
        i++;
    }

    in.close();
    out.close();
    key.close();
}

std::wint_t Vernam::CipherFunc(const std::wint_t& c, const std::wint_t& key){
    return c ^ key;
}