#! /bin/bash
g++ -Wall -std=c++14 -DDEBUG -c -o qr.o qr.cpp
# g++ -Wall -std=c++14 -DDEBUG test_qr.cpp qr.o
g++ -Wall -std=c++14 -DDEBUG test_times.cc qr.o
