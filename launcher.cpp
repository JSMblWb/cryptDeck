#include <iostream>
#include <utility>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <thread>
#include <chrono>

#include "keygen.h"
#include "./ciphers/vernam/vernam.h"
#include "./ciphers/vigenere/vigenere.h"
#include "./ciphers/double-transposition/double-transposition.h"

enum class useType {
	file = 0,
	text = 1
};

enum class startOptions {
	encode = 1,
	decode = 2,
	encodeText = 3,
	decodeText = 4,
	generateKey = 5,
	about = 6,
	exit = 7
};

enum class encodeOptions{
	exit = 1,
	vernam = 2,
	vigenere = 3,
	doubleTransposition = 4,	
};

template <typename T>
void correctInput(std::string q, T& out);
void clearConsole();
void art();
void seqEncode(useType type);
void seqDecode(useType type);
void seqAbout();
void generateKey();
std::pair<std::string, std::string> seqPaths();


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
	3. Зашифровать текст;
	4. Дешифровать текст;
	5. Сгенерировать ключ;
	6. О программе;
	7. Выход.
		)" << std::endl << R"(	)";

		if (isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;

		std::cout << "Ожидание ввода: ";
		int userInput;
		std::cin >> userInput; //НЕ ЗАБЫТЬ ЗДЕСЬ СДЕЛАТЬ ОТЛОВ ЭКСЕПШЕНА НА НЕ ЧИСЛО
		startOptions choice = static_cast<startOptions>(userInput);
		switch (choice) {
			case startOptions::encode:
				seqEncode(useType::file);
				break;
			case startOptions::decode:
				seqDecode(useType::file);
				break;
			case startOptions::encodeText:
				seqEncode(useType::text);
				break;
			case startOptions::decodeText:
				seqDecode(useType::text);
				break;
			case startOptions::generateKey:
				generateKey();
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

void generateKey(){
	int choice;
	int cipher;
	bool running = true;

	while (running) {
		clearConsole();
		art();

		std::cout << "1. Выход\n2. Шифр Вернама\n3. Шифр Виженера\n4. Шифр двойной перестановки\n\nВаш выбор: ";
		std::cin >> choice;
			if (choice == 1)
				running = false;
			if (choice == 2 || choice == 3 || choice == 4){
				int mode;
				std::cout << "1. Выход\n2. Текстовый ключ\n 3. Файл-ключ\n\nВаш выбор: ";
				std::cin >> mode;

				switch (mode){
					case 1:
						running = false;
						break;
					case 2: {
						size_t length;
						bool isLatin;
						std::string key;
						std::wstring wKey;

						correctInput("Введите длину ключа: ", length);
						correctInput("Ключ на русском языке? (0 - да, 1 - на латинском): ", isLatin);

						try {
							KEYGEN keyGen;

							std::string out;
							std::ofstream outFile("./textKey.txt", std::ios::out);
							std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						
							wKey = keyGen.textKey(length, isLatin);
							out = converter.to_bytes(wKey);

							outFile.write(out.data(), out.size());
						
							std::cout << "Ключ сгенерирован: " << out << " | И записан по адресу: ./textKey.txt" << std::endl;

							std::this_thread::sleep_for(std::chrono::seconds(3));
						} catch (const std::runtime_error &e) {
							std::cout << "Ошибка: " << e.what() << std::endl;

							std::this_thread::sleep_for(std::chrono::seconds(3));
						}

						break;
					}
					case 3: {
						size_t length;
						std::string path;
		
						correctInput("Введите длину ключа в байтах: ", length);
						correctInput("Введите путь для записи файла: ", path);

						try {
							KEYGEN keyGen;

							keyGen.byteFileKey(path, length);

							std::cout << "Файл-ключ успешно записан по адресу: " << path << std::endl;

							std::this_thread::sleep_for(std::chrono::seconds(3));
						} catch (const std::runtime_error &e) {
							std::cout << "Ошибка: " << e.what() << std::endl;

							std::this_thread::sleep_for(std::chrono::seconds(3));
						}

						break;
					}
					default: running=false;
				}
			}
	}
}

template <typename T>
void correctInput(std::string question, T& out){
	std::cout << question;
	while(!(std::cin >> out)){
		std::cout << "Некорректный ввод\n" << question;
		std::cin.clear();
	}
}

//=============================================================================== энкод
void seqEncode(useType type) {
	bool funcRunning = true;
	bool isInvalid = false;
	bool isSuccesful = false;
	//std::pair<std::string, std::string> paths = seqPaths();
	//clearConsole();

	while (funcRunning) {
		clearConsole();
		art();
		std::cout << R"(
	Зашифровать:
	1. Выход;
	2. Шифр Вернама;
	3. Шифр Виженера;
	4. Шифр двойной перестановки;
	)";
		if (isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;
		if (isSuccesful) std::cout << "Шифрование выполнено успешно. ", isSuccesful = false;

		int userInput;
		correctInput("Ожидание ввода", userInput);

		encodeOptions choice = static_cast<encodeOptions>(userInput);
		switch (choice) {
			case encodeOptions::vernam: {
				if (type == useType::file){
					std::string outPath;
					std::string inPath;
					std::string keyPath;

					correctInput("Введите путь к файлу, который требуется зашифровать: ", inPath);
					correctInput("Введите путь для записи файла: ", outPath);
					correctInput("Введите путь к файлу-ключу: ", keyPath);

					std::cout << std::endl;

					try{
						Vernam vernamObject;

						vernamObject.EncryptOrDecryptBinary(inPath, outPath, keyPath);
						isSuccesful = true;

						std::cout << "Файл успешно зашифрован по пути: " << outPath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
					catch (const std::runtime_error &e){
						std::cerr << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
					isSuccesful = false;
				} else if (type == useType::text) {
					std::string toEncrypt;
					std::string key;

					std::wstring wToEncrypt;
					std::wstring wKey;

					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

					correctInput("Введите текст для шифрования: ", toEncrypt);
					correctInput("Введите ключ: ", key);

					wToEncrypt = converter.from_bytes(toEncrypt);
					wKey = converter.from_bytes(key);

					try {
						Vernam vernamObject;
						std::wstring wOut;
						std::string out;

						wOut = vernamObject.EncryptOrDecryptText(wToEncrypt, wKey);
						out = converter.to_bytes(wOut);

						std::cout << "Зашифрованный текст: " << out << std::endl;
						std::cout << "Зашифрованный текст в байтах: ";

						for (char c: out){
							std::cout << static_cast<int>(c) << " ";
						}
						std::cout << std::endl;

						isSuccesful = true;

						std::this_thread::sleep_for(std::chrono::seconds(5));
					} catch (const std::runtime_error &e) {
						std::cerr << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				}
				break;
			}

			case encodeOptions::vigenere: {
				if (type == useType::file){
					std::string path;
					std::string outPath;
					std::string keyPath;

					correctInput("Введите путь к файлу, который требуется зашифровать: ", path);
					correctInput("Введите путь для записи файла: ", outPath);
					correctInput("Введите путь к файлу-ключу: ", keyPath);

					try {
						Vigenere vigenere;

						vigenere.EncryptBinary(path, outPath, keyPath);

						std::cout << "Файл успешно зашифрован по пути: " << outPath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				} else if (type == useType::text) {
					std::string text;
					std::string key;

					std::wstring wText;
					std::wstring wKey;

					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

					correctInput("Введите текст для шифрования: ", text);
					correctInput("Введите ключ: ", key);

					wText = converter.from_bytes(text);
					wKey = converter.from_bytes(key);

					try {
						Vigenere vigenere;
						std::wstring wOut;
						std::string out;
						
						wOut = vigenere.EncryptText(wText, wKey);
						out = converter.to_bytes(wOut);

						std::cout << "Зашифрованный текст: " << out << std::endl;
						std::cout << "Зашифрованный текст в байтах: ";

						for (char c: out){
							std::cout << static_cast<unsigned int>(c) << " ";
						}

						std::cout << "\n";

						std::this_thread::sleep_for(std::chrono::seconds(5));
					} catch (const std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				}

				break;
			}
			case encodeOptions::doubleTransposition: {
				if (type == useType::file){
					std::string inPath;
					std::string outPath;

					correctInput("Введите путь к файлу, который требуется зашифровать: ", inPath);
					correctInput("Введите путь для записи файла", outPath);

					try {
						DT dt;

						dt.doubleEncrypt(inPath, outPath);

						std::cout << "Файл успешно зашифрован по пути: " << outPath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e){
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				} else if (type == useType::text) {
					std::string text;
					std::string key1;
					std::string key2;

					correctInput("Введите текст для шифрования: ", text);
					correctInput("Введите первый ключ: ", key1);
					correctInput("Введите второй ключ: ", key2);

					try {
						DT dt;
						std::string out;

						out = dt.doubleEncryptText(text, key1, key2);

						std::cout << "Текст успешно зашифрован: " << out << std::endl;
						std::cout << "Зашифрованный текст в байтах: ";

						for (char c: out){
							std::cout << static_cast<unsigned int>(c) << " ";
						}

						std::cout << "\n";

						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				}

				break;
			}

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
void seqDecode(useType type) {
	bool funcRunning = true;
	bool isInvalid = false;
	bool isSuccesful = false;
	//std::pair<std::string, std::string> paths = seqPaths();

	while (funcRunning) {
		clearConsole();
		art();
		std::cout << R"(
	Дешифровать:
	1. Выход;
	2. Шифр Вернама;
	3. Шифр Виженера;
	4. Шифр двойной перестановки;
	)";

		std::cout << std::endl << std::endl << R"(      )";
		if (isInvalid) std::cout << "Некорректный ввод. ", isInvalid = false;
		if (isSuccesful) std::cout << "Дешифровка выполнена успешно. ", isSuccesful = false;

		int userInput;
		correctInput("Ожидание ввода: ", userInput);

		encodeOptions choice = static_cast<encodeOptions>(userInput);
		switch (choice) {
			case encodeOptions::vernam: {
				if (type == useType::file){
					std::string inPath;
					std::string outPath;
					std::string keyPath;

					correctInput("Введите путь к зашифрованному файлу: ", inPath);
					correctInput("Введите путь для записи файла: ", outPath);
					correctInput("Введите путь к файлу-ключу: ", keyPath);

					try {
						Vernam vernamObject;
						
						vernamObject.EncryptOrDecryptBinary(inPath, outPath, keyPath);

						std::cout << "Файл успешно дешифрован по пути: " << outPath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e){
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				} else if (type == useType::text) {
					std::string in;
					std::string key;

					std::wstring wIn;
					std::wstring wKey;

					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

					correctInput("Введите текст для дешифрования: ", in);
					correctInput("Введите ключ: ", key);

					wKey = converter.from_bytes(key);
					wIn = converter.from_bytes(in);

					try {
						Vernam vernamObject;
						std::wstring wOut;
						std::string out;

						wOut = vernamObject.EncryptOrDecryptText(wIn, wKey);
						out = converter.to_bytes(wOut);

						std::cout << "Расшифрованный текст: " << out << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				}

				break;
				}

			case encodeOptions::vigenere: {
				if (type == useType::file){
					std::string inPath;
					std::string outPath;
					std::string keyPath;

					correctInput("Введите путь к зашифрованному файлу: ", inPath);
					correctInput("Введите путь для записи файла: ", outPath);
					correctInput("Введите путь к файлу-ключу: ", keyPath);

					try {
						Vigenere vigenere;

						vigenere.DecryptBinary(inPath, outPath, keyPath);

						std::cout << "Файл успешно дешифрован по пути: " << outPath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				} else if (type == useType::text) {
					std::string text;
					std::string key;

					std::wstring wText;
					std::wstring wKey;

					std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

					correctInput("Введите текст для дешифрования: ", text);
					correctInput("Введите ключ: ", key);

					wText = converter.from_bytes(text);
					wKey = converter.from_bytes(key);

					try {
						Vigenere vigenere;
						std::wstring wOut;
						std::string out;

						wOut = vigenere.DecryptText(wText, wKey);
						out = converter.to_bytes(wOut);

						std::cout << "Текст успешно расшифрован: " << out << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				}

				break;
			}
			case encodeOptions::doubleTransposition: {
				if (type == useType::text){
					std::string text;
					std::string key1;
					std::string key2;

					correctInput("Введите текст для дешифрования: ", text);
					correctInput("Введите первый ключ: ", key1);
					correctInput("Введите второй ключ: ", key2);

					try {
						DT dt;
						std::string decrypted;
					
						decrypted = dt.doubleDecryptText(text, key1, key2);

						std::cout << "Текст успешно расшифрован: " << decrypted << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e){
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				} else if (type == useType::file){
					std::string inPath;
					std::string outPath;

					correctInput("Введите путь к файлу, который требуется дешифровать: ", inPath);
					correctInput("Введите путь для записи файла: ", outPath);

					try {
						DT dt;

						dt.doubleDecrypt(inPath, outPath);

						std::cout << "Файл успешно расшифрован по пути: " << outPath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					} catch (const std::runtime_error &e) {
						std::cout << "Ошибка: " << e.what() << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(3));
					}
				}
				break;
			}

			case encodeOptions::exit:
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

	Оськин "destroyer1273" Матвей - ;
	Рыбальченко "JSMblWb" Дмитрий - шифр двойной перестановкой, TEA;
	Холодная "freezingg" Алина - ;
	Юрченко "tascarit" Василий - шифр Вернама, шифр Вижинера.
		
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
