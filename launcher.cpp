#include <iostream>

void clearConsole();
void seqEncode();
void seqDecode();
void seqAbout();

enum class startOptions {
	encode = 1,
	decode = 2,
	about = 3,
	exit = 4
};

enum class encodeOptions{
};

enum class decodeOptions{
};

int main() {
	bool mainRunning = true;
	bool isInvalid = false;
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
} //конец мэйна

void clearConsole() {
	std::cout << "\033[H\033[2J" << std::flush;
}

void seqEncode() {
	bool funcRunning = true;
	while (funcRunning) {
		std::cout << R"(
	)"
	} //конец энкода
}

void seqDecode() {
	bool funcRunning = true;
	while (funcRunning) {

	} // конец декода
}

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
} //конец эбаут
