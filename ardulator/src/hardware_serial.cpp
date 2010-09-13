#include "arduino_api.h"
#include <iostream>

using namespace std;

int pin_register[8];

HardwareSerial Serial(-1, -2);

void Print::printNumber(unsigned long, uint8_t) {
    cout << "NYI\n" << endl;
}

void Print::printFloat(double, uint8_t) {
    cout << "NYI\n" << endl;
}

void Print::print(const char[]) {
    cout << "NYI\n" << endl;
}
void Print::print(char, int) {
    cout << "NYI\n" << endl;
}
void Print::print(unsigned char, int) {
    cout << "NYI\n" << endl;
}
void Print::print(int, int) {
    cout << "NYI\n" << endl;
}
void Print::print(unsigned int, int) {
    cout << "NYI\n" << endl;
}
void Print::print(long, int) {
    cout << "NYI\n" << endl;
}
void Print::print(unsigned long, int) {
    cout << "NYI\n" << endl;
}
void Print::print(double, int) {
    cout << "NYI\n" << endl;
}

void Print::println(const char[]) {
    cout << "NYI\n" << endl;
}
void Print::println(char, int) {
    cout << "NYI\n" << endl;
}
void Print::println(unsigned char, int) {
    cout << "NYI\n" << endl;
}
void Print::println(int, int) {
    cout << "NYI\n" << endl;
}
void Print::println(unsigned int, int) {
    cout << "NYI\n" << endl;
}
void Print::println(long, int) {
    cout << "NYI\n" << endl;
}
void Print::println(unsigned long, int) {
    cout << "NYI\n" << endl;
}
void Print::println(double, int) {
    cout << "NYI\n" << endl;
}
void Print::println(void) {
    cout << "NYI\n" << endl;
}

void Print::write(const char *str) {
    cout << "NYI\n" << endl;
}
void Print::write(const uint8_t *buffer, size_t size) {
    cout << "NYI\n" << endl;
}

HardwareSerial::HardwareSerial(int rx, int tx) : _rxen(rx), _txen(tx) {
}

void HardwareSerial::begin(long) {
    cout << "NYI\n" << endl;
}
void HardwareSerial::end() {
    cout << "NYI\n" << endl;
}
uint8_t HardwareSerial::available(void) {
    cout << "NYI\n" << endl;
}
int HardwareSerial::read(void) {
    cout << "NYI\n" << endl;
}
void HardwareSerial::flush(void) {
    cout << "NYI\n" << endl;
}
void HardwareSerial::write(uint8_t) {
    cout << "NYI\n" << endl;
}
