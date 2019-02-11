#include "BitInputStream.hpp"


BitInputStream::BitInputStream(istream & i) : in(i) {
    buf = 0;
    nbits = 0;

    if(i.is_open()){
        eof = 0;
    }
    else{
        eof = 1;    
    }
}

bool BitInputStream::readBit() {
    if(buf == 0){
        //load buffer
        in >> buf;
        if(in.eof()) {
            eof = 1;
        }
        nbits = 0;
    }

    unsigned char mask = 0x1; 
    mask = mask << (7 - nbits);
    mask = mask & buf;
    nbits ++;

    if (nbits == 8){
        in >> buf;    
        if(in.eof()) {
            eof = 1;
        }
        nbits = 0;
    }

    if(mask == 0){
        return false;    
    }

    else {
        return true;    
    };
}
