#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

#define BITS_IN_BYTE 8
#define DEF_MASK 0x1

using namespace std;

class BitInputStream {
private:
    char buf;
    int nbits;
    istream & in;

public:
    BitInputStream(istream & i);
    bool readBit();
    unsigned char getInt();
};

#endif // BITINPUTSTREAM_HPP
