#include <cstdio>
#include <cstring>
#include <stdexcept>

class SimpleStringB;

// 1st string type: buffer for string as data member array
class SimpleStringA {
    friend SimpleStringB; // for conversion
public:
    SimpleStringA() = default;
    ~SimpleStringA() = default;
    // copy
    SimpleStringA(const SimpleStringA& source): 
    length{source.length} 
    {
        std::strncpy(buffer, source.buffer, max_size);
    }

    SimpleStringA& operator=(const SimpleStringA& source) {
        if (&source == this)
            return *this;
        length = source.length;
        std::strncpy(buffer, source.buffer, max_size);

        return *this;
    }

    // move
    SimpleStringA(SimpleStringA&& source) noexcept: 
    length{source.length} 
    {
        std::memmove(buffer, source.buffer, max_size);
        source.length = 0;
    }

    SimpleStringA& operator=(SimpleStringA&& source) noexcept {
        if (&source == this)
            return *this;
        length = source.length;
        std::memmove(buffer, source.buffer, max_size);
        source.length = 0;

        return *this;
    }

    // // converter for SimpleStringA
    // // need class declarations to be transferred on a header file for this to work
    // SimpleStringA(const SimpleStringB& string_b) {
    //     length{string_b.length};
    //     if (length+2 > max_size) 
    //         throw std::length_error("Length error: conversion from SimpleStringB to SimpleStringA impossible. "
    //                                 "SimpleStringB exceeds size allowed for SimpleStringA.\n");
    //     std::strcpy(buffer, string_b.buffer);
    // }

    // utilities
    bool appendLine(const char* input) {
        size_t input_len = strlen(input);
        if (length+input_len > max_size-2) {
            throw std::length_error("appended line exceeds max_size allowed\n");
            return false;
        }
        std::strncpy(buffer+length, input, input_len);
        length += input_len;
        buffer[length] = '\n';
        length +=1;
        buffer[length+1] = 0;

        return true;
    }

    void print() const {
        printf("%s", buffer);
    }

protected:
    //size_t max_size, length;
    const static size_t max_size = 200;
    size_t length;
    char buffer[max_size];
};

// 2nd string type: buffer for string as data member pointer-to-array
class SimpleStringB {
    friend SimpleStringA; // for conversion
public:
    SimpleStringB() = default;

    SimpleStringB(size_t size): 
    max_size{size}, length{0}
    {
        if (max_size == 0)
            throw std::length_error("size of string cannot be null\n");

        buffer = new char[max_size];
    }

    ~SimpleStringB() {
        delete[] buffer;
    }

    // copy
    SimpleStringB(const SimpleStringB& source): 
    max_size{source.max_size}, length{source.length}, buffer{new char[source.max_size]}
    {
        std::strcpy(buffer, source.buffer);
    }

    SimpleStringB& operator=(const SimpleStringB& source) {
        if (&source == this)
            return *this;
        max_size = source.max_size;
        length = source.length;

        delete[] buffer;
        buffer = new char[max_size];
        std::strcpy(buffer, source.buffer);

        return *this;
    }

    // move
    SimpleStringB(SimpleStringB&& source) noexcept: 
    max_size{source.max_size}, length{source.length}, buffer{source.buffer}
    {
        source.max_size = 0;
        source.length = 0;
        source.buffer = nullptr;
    }

    SimpleStringB& operator=(SimpleStringB&& source) noexcept {
        if (&source == this)
            return *this;

        max_size = source.max_size;
        length = source.length;

        delete[] buffer;
        buffer = source.buffer;

        source.max_size = 0;
        source.length = 0;
        source.buffer = nullptr;

        return *this;
    }

    // converter constructors
    SimpleStringB(const SimpleStringA& string_a):
    max_size{string_a.max_size}, length{string_a.length}, buffer{new char[string_a.max_size]} {
        std::strcpy(buffer, string_a.buffer);
    }

    // utilities
    bool appendLine(const char* input) {
        size_t input_len = strlen(input);
        if (length+input_len > max_size-2) {
            throw std::length_error("appended line exceeds max_size allowed\n");
            return false;
        }
        std::strcpy(buffer+length, input);
        length = strlen(buffer);

        return true;
    }

    void print() const {
        printf("%s", buffer);
    }

protected:
    size_t max_size, length;
    char* buffer;
};

int main() {
    SimpleStringA my_stringA;
    try {
        my_stringA.appendLine("This is an example of SimpleStringA.\n");
        my_stringA.print();
        my_stringA.appendLine("The problem is these are already being hit, as can be seen above-so it's questionable if they're being saved for some new operation. "
                              "Secondly, the new 150-series reserve brigades Zelensky was putting together for future operations have been marred with problems. "
                              "A new report highlighted how 1000 men have already deserted the 155th of this series, not to mention confirming that the brigades have already been utilized merely to fill gaps in critical areas of the Donbass theater."
        );
        my_stringA.print();
    } catch(std::exception& e) {
        printf("Exception caught: %s\n", e.what());
    }

    SimpleStringB my_stringB(150);
    try {
        my_stringB.appendLine("This is an example of SimpleStringB.\n");
        my_stringB.print();
        my_stringB.appendLine("The problem is these are already being hit, as can be seen above-so it's questionable if they're being saved for some new operation. "
                              "Secondly, the new 150-series reserve brigades Zelensky was putting together for future operations have been marred with problems. "
                              "A new report highlighted how 1000 men have already deserted the 155th of this series, not to mention confirming that the brigades have already been utilized merely to fill gaps in critical areas of the Donbass theater."
        );
        my_stringB.print();
    } catch(std::exception& e) {
        printf("Exception caught: %s\n", e.what());
    }

    printf("Conversions: \n");
    //SimpleStringA B_to_A(my_stringB);
    //B_to_A.print();
    SimpleStringB A_to_B(my_stringA);
    A_to_B.print();
}