#!/bin/bash


g++ -fPIC -shared -o libs/build/libVernam.so ciphers/vernam/vernam.cpp
g++ -fPIC -shared -o libs/build/libVigenere.so ciphers/vigenere/vigenere.cpp

g++ -fPIC -shared -o libs/build/libKeygen.so keygen.cpp
g++ launcher.cpp -o cryptDeck libs/build/libKeygen.so libs/build/libVernam.so libs/build/libVigenere.so
