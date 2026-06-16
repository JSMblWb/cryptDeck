#include "vigenere.h"

std::wstring Vigenere::EncryptText(const std::wstring& text, const std::wstring& key){
    std::wstring out = std::wstring(text.size(), 0);
	size_t shift = 0;

	if (text.size() == 0 || key.size() == 0)
		throw std::runtime_error("Длина текста или ключа равна нулю");

    for (size_t i = 0; i < text.size(); i++){
		wint_t pos = text[i];

		if ((pos >= RU_START && pos <= RU_END) || (pos >= RU_UP_START && pos <= RU_UP_END) || (pos >= LAT_START && pos <= LAT_END) || (pos >= LAT_UP_START && pos <= LAT_UP_END)) {
			out[i] = Encrypt(text[i], key[(i-shift) % key.size()]);
    	} else {
			out[i] = text[i];
			shift++;
		}
	}

    return out;
}

std::wstring Vigenere::DecryptText(const std::wstring& encrypted, const std::wstring& key){
    std::wstring out = std::wstring(encrypted.size(), 0);
    
	if (encrypted.size() == 0 || key.size() == 0)
		throw std::runtime_error("Длина текста или ключа равна нулю");

    for (size_t i = 0; i < encrypted.size(); i++){
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
	
	if (length == 0)
		throw std::runtime_error("Длина генерируемого ключа не может быть равна нулю");

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

void Vigenere::EncryptTextFile(const std::string& path, const std::string& out_path, const std::wstring& key){
    std::wifstream in(path, std::ios::binary);;
    std::wofstream out(out_path, std::ios::binary);
    wchar_t c;
    size_t i = 0;

    if (!in.is_open() || !out.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    while (in.get(c)){
        std::streampos pos = in.tellg();

        out.put(Encrypt(c, key[i % key.size()]));

        i++;
    }

    in.close();
    out.close();
}

void Vigenere::DecryptTextFile(const std::string& path, const std::string& out_path, const std::wstring& key){
    std::wifstream in(path, std::ios::binary);;
    std::wofstream out(out_path, std::ios::binary);
    wchar_t c;
    size_t i = 0;

    if (!in.is_open() || !out.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    while (in.get(c)){
        std::streampos pos = in.tellg();

        out.put(Decrypt(c, key[i % key.size()]));

        i++;
    }

    in.close();
    out.close();
}

void Vigenere::EncryptBinary(const std::string& path, const std::string& out_path, const std::string& key_path){
    std::ifstream in(path, std::ios::binary);
    std::ifstream key(key_path, std::ios::binary);
    std::ofstream out(out_path, std::ios::binary);
    size_t i = 0, maxKeySize;

    if (!in.is_open() || !key.is_open() || !out.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    key.seekg(0, std::ios::end);
    maxKeySize = key.tellg();
    key.seekg(0, std::ios::beg);

    if (maxKeySize == 0)
        throw std::runtime_error("Файл ключа пустой");

    unsigned char byte, keyByte;

    while(in.read(reinterpret_cast<char*>(&byte), 1) && (key.seekg(i % maxKeySize) && key.read(reinterpret_cast<char*>(&keyByte), 1))){        
        byte = (byte + keyByte) % 256;
        out.write(reinterpret_cast<const char*>(&byte), 1);
        i++;
    }

    in.close();
    out.close();
    key.close();
}

void Vigenere::DecryptBinary(const std::string& path, const std::string& out_path, const std::string& key_path){
    std::ifstream in(path, std::ios::binary);
    std::ifstream key(key_path, std::ios::binary);
    std::ofstream out(out_path, std::ios::binary);
    size_t i = 0, maxKeySize;

    if (!in.is_open() || !key.is_open() || !out.is_open())
        throw std::runtime_error("Ошибка открытия файла");

    key.seekg(0, std::ios::end);
    maxKeySize = key.tellg();
    key.seekg(0, std::ios::beg);

    if (maxKeySize == 0)
        throw std::runtime_error("Файл ключа пустой");

    unsigned char byte, keyByte;

    while(in.read(reinterpret_cast<char*>(&byte), 1) && (key.seekg(i % maxKeySize) && key.read(reinterpret_cast<char*>(&keyByte), 1))){
        byte = (byte - keyByte + 256) % 256;

        out.write(reinterpret_cast<const char*>(&byte), 1);
        i++;
    }

    in.close();
    out.close();
    key.close();
}

void Vigenere::ByteFileKeyGenerator(std::string&path, size_t length){
    if (length == 0)
        throw std::runtime_error("Длина ключа не может быть равна нулю");

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

wchar_t Vigenere::Encrypt(const std::wint_t& c, const std::wint_t& key){
    wint_t letter = c;
    wint_t keyAlphLength = (key >= RU_START) ? RU_START : ((key >= RU_UP_START) ? RU_UP_START : ((key >= LAT_START) ? LAT_START : ((key >= LAT_UP_START) ? LAT_UP_START : SYMBOLS_START)));
    int charType = (letter >= RU_START && letter <= RU_END) ? 0 : ((letter >= RU_UP_START && letter <= RU_UP_END) ? 1 : ((letter >= LAT_START && letter <= LAT_END) ? 2 : ((letter >= LAT_UP_START && letter <= LAT_UP_END) ? 3 : 4)));
	size_t alphLen = (charType == 0) ? RU_END-RU_START : ((charType == 1) ? RU_UP_END-RU_UP_START : ((charType == 2) ? LAT_END-LAT_START : LAT_UP_END-LAT_UP_START)) + 1;
    wint_t result = letter + ((key - keyAlphLength) % alphLen);

    if (key == 1025)
        result = letter + (key - RU_UP_START);
    if (letter == 1025)
        charType = 1;

    switch (charType){
        case 0: return (result > RU_END) ? (result - (RU_END-RU_START+1)) : result;
        case 1: return (result > RU_UP_END) ? (result - (RU_UP_END-RU_UP_START+1)) : result;
        case 2: return (result > LAT_END) ? (result - (LAT_END-LAT_START+1)) : result;
        case 3: return (result > LAT_UP_END) ? (result - (LAT_UP_END-LAT_UP_START+1)) : result;
        default: return letter;
    }
}

wchar_t Vigenere::Decrypt(const std::wint_t& c, const std::wint_t& key){
    wint_t letter = c;
    wint_t keyAlphLength = (key >= RU_START) ? RU_START : ((key >= RU_UP_START) ? RU_UP_START : ((key >= LAT_START) ? LAT_START : ((key >= LAT_UP_START) ? LAT_UP_START : SYMBOLS_START)));
    int charType = (letter >= RU_START && letter <= RU_END) ? 0 : ((letter >= RU_UP_START && letter <= RU_UP_END) ? 1 : ((letter >= LAT_START && letter <= LAT_END) ? 2 : ((letter >= LAT_UP_START && letter <= LAT_UP_END) ? 3 : 4)));
    size_t alphLen = (charType == 0) ? RU_END-RU_START : ((charType == 1) ? RU_UP_END-RU_UP_START : ((charType == 2) ? LAT_END-LAT_START : LAT_UP_END-LAT_UP_START)) + 1;
	wint_t result = letter - ((key - keyAlphLength) % alphLen);
    
    if (key == 1025)
        result = letter - (key - RU_UP_START);
    if (letter == 1025)
        charType = 1;

    switch (charType){
        case 0: return (result < RU_START) ? (result + (RU_END-RU_START+1)) : result;
        case 1: return (result < RU_UP_START) ? (result + (RU_UP_END-RU_UP_START+1)) : result;
        case 2: return (result < LAT_START) ? (result + (LAT_END-LAT_START+1)) : result;
        case 3: return (result < LAT_UP_START) ? (result + (LAT_UP_END-LAT_UP_START+1)) : result;
        default: return letter;
    }
}
