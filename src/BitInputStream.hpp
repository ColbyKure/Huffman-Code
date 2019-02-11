#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

using namespace std;

class BitInputStream {
private:
    char buf;
    int nbits;
    istream & in;
    bool eof;

public:
    BitInputStream(istream & i);
    bool readBit();

    
};

#endif // BITINPUTSTREAM_HPP
