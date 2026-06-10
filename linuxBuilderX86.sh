#!/bin/bash


g++ -fPIC -shared -o libs/libVernamX86.so ciphers/vernam/vernam.cpp


g++ launcher.cpp -o cryptDeckArm libs/libVernamX86.so
