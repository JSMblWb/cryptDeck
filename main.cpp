#include <iostream>
#include <locale>

#include "ciphers\vigenere\vigenere.h"

int main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Vigenere vigenere;

    vigenere.DecryptTextFile("C:\\Users\\tscrt\\Desktop\\problems\\cryptDeck\\textfile.txt", L"ПОНОСДЕДА");
}