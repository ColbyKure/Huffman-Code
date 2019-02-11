#include "BitOutputStream.hpp"

BitOutputStream::BitOutputStream(ostream & o) : out(o) {
    buf = 0;
    nbits = 0;
}

void BitOutputStream::writeBit(bool bit) {
    if(nbits == 8){
        flush();
        buf = 0;
    }
    
    if (bit == 1){ //true
        byte mask = 0x1; 
        mask = mask << (7 - nbits);
        mask = mask | buf;   
    }
    nbits++;
    return;
}

void BitOutputStream::flush(){
    out << buf;
}
