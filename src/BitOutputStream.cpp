#include <iostream>

#include "BitOutputStream.hpp"

BitOutputStream::BitOutputStream(ostream & o) : out(o) {
    buf = 0; //8-bit buffer for stream
    nbits = 0; //number of bits in buffer
}

void BitOutputStream::writeBit(bool bit) {
    //cout << "in writeBits, nbits = " << nbits << endl;
    //free next bit and then set it
    buf = buf << 1;
    buf = buf | bit;

    //increment number of bits in buf
    nbits++;

    //if full flush
    if(nbits == BITS_IN_BYTE){
        flush();
    }
}

void BitOutputStream::flush(){
    //fill buf with 0's
    while(nbits != BITS_IN_BYTE) {
        buf = buf << 1;
        nbits++;
    }
    //cout << "flushing buf, buf = " << (int)buf << endl;
    /*if(buf != 0) {
        unsigned char mask = 0x80;
        for(int i = 0; i < 8; ++i) {
            bool asdf = (bool)(mask & buf);
            cout << (int)asdf;
            mask = mask >> 1;
        }
        cout << endl;
        cout << endl;
    }*/
    //put onto ostream
    out.put(buf);
    out.flush();

    //reset buf and size
    buf = 0;
    nbits = 0;
}
