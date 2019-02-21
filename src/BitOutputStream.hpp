#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

#define BITS_IN_BYTE 8

using namespace std;

class BitOutputStream {
private:
    unsigned char buf;      //8-bit buffer for our bit stream
    unsigned int nbits;     //number of bits in buf
    ostream & out;          //buffer to flush to

public:
    BitOutputStream(ostream & o);
    void writeBit(bool bit);
    void flush();
};

#endif // BITOUTPUTSTREAM_HPP
