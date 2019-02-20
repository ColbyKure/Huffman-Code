#include <iostream>

#include "BitOutputStream.hpp"

BitOutputStream::BitOutputStream(ostream & o) : out(o) {
    buf = 0;
    nbits = 0;
}

void BitOutputStream::writeBit(bool bit) {
    if(nbits == BITS_IN_BYTE){
        flush();
    }

    buf = buf << 1;
    buf = buf | bit;

    /*if(bit == 1){ //true
        unsigned char mask = DEF_MASK;
        mask = mask << (BITS_IN_BYTE - nbits - 1);
        buf = mask | buf;
    }*/

    nbits++;
}

void BitOutputStream::flush(){
    out.put(buf);
    out.flush();
    buf = 0;
    nbits = 0;
}
