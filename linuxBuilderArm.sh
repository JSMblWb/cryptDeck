#!/bin/bash


g++ -fPIC -shared -o libs/libVernamArm.so ciphers/vernam/vernam.cpp
g++ -fPIC -shared -o libs/libVigenereArm.so ciphers/vigenere/vigenere.cpp

g++ launcher.cpp -o cryptDeckArm libs/libVernamArm.so
