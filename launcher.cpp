#include <iostream>
#include <dlfcn.h>
#include <vector>
#include <fstream>
#include <cstring>

using EncryptFunc = bool(*)(const unsigned char*, size_t, unsigned char*, size_t*);
using DecryptFunc = bool(*)(const unsigned char*, size_t, unsigned char*, size_t*);
using InitFunc = bool(*)(const char*, size_t);

EncryptFunc encrypt = nullptr;
DecryptFunc decrypt = nullptr;
InitFunc init_cipher = nullptr;

void clearConsole();
void seqEncode();
void seqDecode();
void seqAbout();
bool loadCipherLibrary(const char* libPath);
std::vector<unsigned char> readFile(const std::string& filename);

enum class startOptions {
	encode = 1,
	decode = 2,
	about = 3,
	exit = 4
};

enum class encodeOptions{
	swTail = 0
};

enum class decodeOptions{
	swAuto = 0
};

//=============================================================================== мэйн
int main() {
	bool mainRunning = true;
	bool isInvalid = false;

	//загружаем библиотеку
	if (!loadCipherLibrary("./libcipher.so")) {  // Путь к вашей .so
		std::cerr << "Не удалось загрузить библиотеку шифрования." << std::endl;
		return 1;
	}

	//инициализация (если требуется)
	if (init_cipher) {
		const char* key = "your_secret_key";
		if (!init_cipher(key, strlen(key))) {
			std::cerr << "Ошибка инициализации шифрования." << std::endl;
			return 1;
		}
	}

	while (mainRunning) {
		clearConsole();
		std::cout << R"(
	                    __  ___          __  
	 __________ _____  / /_/ _ \___ ____/ /__
	/ __/ __/ // / _ \/ __/ // / -_) __/  '_/
	\__/_/  \_, / .__/\__/____/\__/\__/_/\_\ 
	       /___/_/                           
		)" << std::endl << R"(
	Для продолжения впишите номер одной из следующих опций:
	1. Зашифровать файл;
	2. Дешифровать файл;
	3. О программе;
	4. Выход.
		)" << std::endl << R"(	)";

		if(isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;

		std::cout << "Ожидание ввода: ";
		int userInput;
		std::cin >> userInput; //НЕ ЗАБЫТЬ ЗДЕСЬ СДЕЛАТЬ ОТЛОВ ЭКСЕПШЕНА НА НЕ ЧИСЛО
		startOptions choice = static_cast<startOptions>(userInput);
		switch (choice) {
			case startOptions::encode:
				seqEncode();
				break;
			case startOptions::decode:
				seqDecode();
				break;
			case startOptions::about:
				seqAbout();
				break;
			case startOptions::exit:
				mainRunning = false;
				break;
			default:
				isInvalid = true;
				break;
		} //конец свича
	} //конец цикла
	clearConsole();
	std::cout << R"(
	  _   ___             __       __  _____________  ____
	 | | / (_)  _____ _  / /__ _  / / / /_  __/ __/ |/ / /
	 | |/ / / |/ / _ `/ / / _ `/ / /_/ / / / / _//    /_/ 
	 |___/_/|___/\_,_/ /_/\_,_/  \____/ /_/ /___/_/|_(_)  )" << std::endl << std::endl;
	return 0;
}
//=============================================================================== очистка
void clearConsole() {
	std::cout << "\033[H\033[2J" << std::flush;
}
//=============================================================================== энкод
void seqEncode() {
	if (!encrypt) {
		std::cout << "Библиотека шифрования не загружена!" << std::endl;
		std::cin.get();
		return;
	}

	std::string inputFile, outputFile;

	std::cout << "Введите путь к входному файлу: ";
	std::cin >> inputFile;

	std::cout << "Введите путь к выходному файлу: ";
	std::cin >> outputFile;

	try {
		//читаем входной файл
		std::vector<unsigned char> inputData = readFile(inputFile);

		//пробуем с разными размерами буфера
		size_t outputSize = inputData.size() + 1024; //запас для шифрования
		std::vector<unsigned char> outputData(outputSize);

		//вызываем функцию шифрования из .so
		if (encrypt(inputData.data(), inputData.size(), outputData.data(), &outputSize)) {
			outputData.resize(outputSize);
			outputData.resize(outputSize);
			writeFile(outputFile, outputData.data(), outputData.size());
			std::cout << "Файл успешно зашифрован!" << std::endl;
		} else {
			std::cout << "Ошибка при шифровании!" << std::endl;
		}
	} catch (const std::exception& e) {
		std::cout << "Ошибка: " << e.what() << std::endl;
	}

	std::cout << "Нажмите Enter для продолжения...";
	std::cin.ignore();
	std::cin.get();


	bool funcRunning = true;
	bool isTailOn = true
	while (funcRunning) {
		std::cout << R"(
	Пункты меню:
	1.
	)"
	isTailOn ? std::cout << "0. Выключить генерацию хвоста для авто дешифровки" : std::cout << "0. Включить генерацию хвоста для авто дешифровки";
	
	}
}
//=============================================================================== декод
void seqDecode() {
	if (!decrypt) {
		std::cout << "Библиотека дешифрования не загружена!" << std::endl;
		std::cin.get();
		return;
	}

	std::string inputFile, outputFile;

	std::cout << "Введите путь к зашифрованному файлу: ";
	std::cin >> inputFile;

	std::cout << "Введите путь к выходному файлу: ";
	std::cin >> outputFile;

	try {
		//аналогично encode
		std::vector<unsigned char> inputData = readFile(inputFile);
 
		size_t outputSize = inputData.size() + 1024;
		std::vector<unsigned char> outputData(outputSize);

		if (decrypt(inputData.data(), inputData.size(), outputData.data(), &outputSize)) {
			outputData.resize(outputSize);
			writeFile(outputFile, outputData.data(), outputData.size());
			std::cout << "Файл успешно дешифрован." << std::endl;
		} else {
			std::cout << "Ошибка при дешифровании." << std::endl;
		}
	} catch (const std::exception& e) {
		std::cout << "Ошибка: " << e.what() << std::endl;
	}

	std::cout << "Нажмите Enter для продолжения...";
	std::cin.ignore();
	std::cin.get();


	bool funcRunning = true;
	while (funcRunning) {

	} // конец декода
}
//=============================================================================== эбаут
void seqAbout() {
	clearConsole();
	std::cout << R"(
			О программе.

	cryptDeck - сборник программ для кодирвоания
		    и декодирования файлов, созданный
		    в рамках расчётно-графической
		    работы №1 по дисциплине
		    "Программирование" из учебной
		    программы НГТУ НЭТИ.


		     Участники проекта.

	Рыбальченко "JSMblWb" Дмитрий - шифр двойной перестановки, TEA;
	Юрченко "tascarit" Василий - ;
	Оськин "destroyer1273" Матвей - ;
	Холодная "freezingg" Алина - ;
		
	Для выхода введите 1: )";
	int waiter; //ПОТОМ ПЕРЕДЕЛАТЬ ЭТО
	std::cin >> waiter;
}
//=============================================================================== загрузка либ
bool loadCipherLibrary(const char* libPath) {
	void* handle = dlopen(libPath, RTLD_LAZY);
	if (!handle) {
		std::cerr << "Невозможно открыть библиотеку: " << dlerror() << std::endl;
		return false;
	}
    
	//очищаем ошибки
	dlerror();

	//загружаем функции
	encrypt = (EncryptFunc)dlsym(handle, "encrypt");
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		std::cerr << "Cannot load symbol encrypt: " << dlsym_error << std::endl;
		return false;
	}

	decrypt = (DecryptFunc)dlsym(handle, "decrypt");
	dlsym_error = dlerror();
	if (dlsym_error) {
		std::cerr << "Cannot load symbol decrypt: " << dlsym_error << std::endl;
		return false;
	}

	init_cipher = (InitFunc)dlsym(handle, "init_cipher");
	//init_cipher может быть опциональной

	return true;
}
//=============================================================================== ридфайл
std::vector<unsigned char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Cannot open file: " + filename);
	}

	return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
//=============================================================================== viva la uten
