#!/bin/bash


g++ -fPIC -shared -o libs/libVernamArm.so ciphers/vernam/vernam.cpp


g++ launcher.cpp -o cryptDeckArm libs/libVernamArm.so
