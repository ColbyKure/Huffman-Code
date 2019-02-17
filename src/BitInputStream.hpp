#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

#define BITS_IN_BYTE 8
#define DEF_MASK 0x1

using namespace std;

class BitInputStream {
private:
    unsigned char buf;
    unsigned int nbits;
    istream & in;

public:
    BitInputStream(istream & i);
    bool readBit();
};

#endif // BITINPUTSTREAM_HPP
