#!/bin/bash


g++ -fPIC -shared -o ciphers/vernam/libvernam.so ciphers/vernam/vernam.cpp


g++ launcher.cpp -o cryptDeck ciphers/vernam/libvernam.so
