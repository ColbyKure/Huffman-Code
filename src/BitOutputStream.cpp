#include "BitOutputStream.hpp"

BitOutputStream::BitOutputStream(ostream & o) : out(o) {
    buf = 0;
    nbits = 0;
}

void BitOutputStream::writeBit(bool bit) {
    if(nbits == BITS_IN_BYTE){
        flush();
    }

    if(bit == 1){ //true
        unsigned char mask = DEF_MASK;
        mask = mask << (BITS_IN_BYTE - nbits - 1);
        mask = mask | buf;
    }

    nbits++;
    return;
}

void BitOutputStream::flush(){
    out.put(buf);
    out.flush();
    buf = 0;
    return;
}
