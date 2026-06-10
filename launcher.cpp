#include <iostream>
#include <utility>
#include <string>
#include <filesystem>
#include "./ciphers/vernam/vernam.h"

void clearConsole();
void art();
void seqEncode();
void seqDecode();
void seqAbout();
std::pair<std::string, std::string> seqPaths();

enum class startOptions {
	encode = 1,
	decode = 2,
	about = 3,
	exit = 4
};

enum class encodeOptions{
	exit = 1,
	vernam = 2,
	vigenere = 3
	
};

enum class decodeOptions{
	exit = 1,
	vernam = 2,
	vigenere = 3
};

//=============================================================================== мэйн
int main() {
	bool mainRunning = true;
	bool isInvalid = false;

	while (mainRunning) {
		clearConsole();
		art();
		std::cout << R"(
	Главное меню:
	1. Зашифровать файл;
	2. Дешифровать файл;
	3. О программе;
	4. Выход.
		)" << std::endl << R"(	)";

		if (isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;

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
//=============================================================================== арт
void art() {
	std::cout << R"(
                            __  ___          __
         __________ _____  / /_/ _ \___ ____/ /__
        / __/ __/ // / _ \/ __/ // / -_) __/  '_/
        \__/_/  \_, / .__/\__/____/\__/\__/_/\_\
               /___/_/
	)" << std::endl;

}
//=============================================================================== энкод
void seqEncode() {
	bool funcRunning = true;
	bool isInvalid = false;
	bool isSuccesful = false;
	std::pair<std::string, std::string> paths = seqPaths();

	while (funcRunning) {
		clearConsole();
		art();
		std::cout << R"(
	Зашифровать:
	1. Выход;
	2. Шифр Вернама;
	3. Шифр Вижинера;

	)";
		if (isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;
		if (isSuccesful) std::cout << "Шифровка выполнена успешно. ", isSuccesful = false;

		std::cout << "Ожидание ввода: ";
		int userInput;
		std::cin >> userInput; //НЕ ЗАБЫТЬ ЗДЕСЬ СДЕЛАТЬ ОТЛОВ ЭКСЕПШЕНА НА НЕ ЧИСЛО

		encodeOptions choice = static_cast<encodeOptions>(userInput);
		switch (choice) {
			case encodeOptions::vernam: {
				const std::string keyPath = "./keyVernam";
				Vernam vernamObject;
				//вот здесь бы спросить длину пути
				//vernamObject.ByteFileKeyGenerator(keyPath, 32);
				//vernamObject.EncryptOrDecryptBinary(paths.first, paths.second, "./keyVernam");
				isSuccesful = true;
				break;
			}

			case encodeOptions::vigenere:
				std::cout << "vigenere";
				break;

			case encodeOptions::exit:
				funcRunning = false;
				break;

			default:
				isInvalid = true;
				break;
		} //конец свича
	}//конец цикла
}
//=============================================================================== декод
void seqDecode() {
	bool funcRunning = true;
	bool isInvalid = false;
	bool isSuccesful = false;
	std::pair<std::string, std::string> paths = seqPaths();

	while (funcRunning) {
		clearConsole();
		art();
		std::cout << R"(
	Дешифровать:
	1. Выход;
	2. Шифр Вернама;
	3. Шифр Вижинера;
	)";

		std::cout << std::endl << std::endl << R"(      )";
		if (isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;
		if (isSuccesful) std::cout << "Дешифровка выполнена успешно. ", isSuccesful = false;

		std::cout << "Ожидание ввода: ";
		int userInput;
		std::cin >> userInput; //НЕ ЗАБЫТЬ ЗДЕСЬ СДЕЛАТЬ ОТЛОВ ЭКСЕПШЕНА НА НЕ ЧИСЛО

		decodeOptions choice = static_cast<decodeOptions>(userInput);
		switch (choice) {
			case decodeOptions::vernam: {
				Vernam vernamObject;
				break;
				}

			case decodeOptions::vigenere:
				std::cout << "vigenere";
				break;

			case decodeOptions::exit:
				funcRunning = false;
				break;

			default:
				isInvalid = true;
				break;
		} //конец свича
	}//конец цикла
}
//=============================================================================== эбаут
void seqAbout() {
	clearConsole();
	art();
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
	Юрченко "tascarit" Василий - шифр Вернама, шифр Виженера;
	Оськин "destroyer1273" Матвей - ;
	Холодная "freezingg" Алина - ;
		
	Для выхода введите 1: )";
	int waiter; //ПОТОМ ПЕРЕДЕЛАТЬ ЭТО
	std::cin >> waiter;
}
//=============================================================================== спрашиватель пути
std::pair<std::string, std::string> seqPaths() {
	bool isInvalid = false;
	while (true) {
		clearConsole();
		art();
		std::cout << R"(
	Введите полный или относительный (с началом .) путь до желаемого файла.

	)";
		if (isInvalid) std::cout << "Такого файла не существует. ", isInvalid = false;
		std::cout << "Ожидание ввода: ";
		std::string inputPath, outputPath;
		std::cin >> inputPath;
		if (std::filesystem::exists(inputPath)) {
			size_t pos = inputPath.rfind('/') + 1;
			outputPath = inputPath.insert(pos, "cd-handled-");
			return {inputPath, outputPath};
		} else isInvalid = true;
	}
}
//=============================================================================== viva la uten
