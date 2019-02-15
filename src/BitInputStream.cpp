#include "BitInputStream.hpp"

BitInputStream::BitInputStream(istream & i) : in(i) {
    buf = BITS_IN_BYTE;
    nbits = 0;
}

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

unsigned char BitInputStream::getInt() {
    unsigned char freq = -1;
    in >> freq;
    return freq;
}
