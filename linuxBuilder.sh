#!/bin/bash


g++ -fPIC -shared -o libs/build/libVernam.so ciphers/vernam/vernam.cpp
g++ -fPIC -shared -o libs/build/libVigenere.so ciphers/vigenere/vigenere.cpp

g++ launcher.cpp -o cryptDeck libs/build/libVernam.so libs/build/libVigenere.so
