#include "BitInputStream.hpp"

#define BITS_IN_BYTE 8

BitInputStream::BitInputStream(istream & i) : buf(0), nbits(8), in(i) { }

bool BitInputStream::readBit() {
    //if full get next 8 bits
    if (nbits == BITS_IN_BYTE) {
        buf = in.get();
        nbits = 0;
    }
 
    unsigned char mask = buf >> (BITS_IN_BYTE - nbits - 1);

    nbits++;

    return (mask & DEF_MASK);

    /*if(mask != 0) {
        return true;
    }
    else {
        return false;
    }*/
}

