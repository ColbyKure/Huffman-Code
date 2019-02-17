#include "BitInputStream.hpp"

#define BITS_IN_BYTE 8

BitInputStream::BitInputStream(istream & i) : buf(0), nbits(8), in(i) { }

bool BitInputStream::readBit() {
    if (nbits == BITS_IN_BYTE) {
        buf = in.get();
        nbits = 0;
    }

    unsigned char mask = DEF_MASK; 
    mask = mask << (BITS_IN_BYTE - nbits - 1);
    mask = mask & buf;
    nbits++;

    if(mask != 0) {
        return true;
    }
    else {
        return false;
    }
}

