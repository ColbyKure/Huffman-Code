#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitInputStream.hpp"

#define MAX_CHAR 256

using namespace std;

void print_usage(char ** argv) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " INFILE OUTFILE [-b]" << endl;
    cout << "Command-line flags:" << endl;
    cout << "  -b: switch to bitwise mode" << endl;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its header,
 * and produces an uncompressed version in outfile.
 * For debugging purposes, uses ASCII '0' and '1' rather than bitwise I/O.
 */
void uncompressAscii(const string & infile, const string & outfile) {
    //open file 
    ifstream in;
    const char * input = infile.c_str();
    in.open(input, ios::binary);

    //check if file actually opened 
    if(!in.is_open()) {
        cerr << "input file was not opened...\n";
        //return;
    }
    
    //find beginning of stream
    in.seekg(0, ios_base::beg);

    //read lines from header 
    vector<int> freqs(MAX_CHAR, 0); //one slot per ascii value = 0
    int nextInt;
    for (int i = 0; i < MAX_CHAR; i++) {
    	in >> nextInt;
    	freqs[i] = nextInt;
    }

    //build tree
    HCTree tree;
    tree.build(freqs);

    //open out stream
    const char * output = outfile.c_str();
    ofstream out(output);
    if(!out.is_open()){
        cerr << outfile << " not opened!\n";
	    //return;
    }
    
    //decode all characters in the file now
    unsigned char nextChar; //for reading output
    while(1) {
    	nextChar = tree.decode(in);
	    if (in.eof()){
	        break;
	    }
	    out << nextChar;
    }

    //close files
    if(out.is_open()){
    	out.close();
    }
    if(in.is_open()){
    	in.close();
    }

    //return;
}


/**
 * Reads infile, constructs a Huffman coding tree based on its header,
 * and produces an uncompressed version in outfile.
 * Uses bitwise I/O.
 */
void uncompressBitwise(const string & infile, const string & outfile) {
    //open file 
    filebuf inF;
    const char * input = infile.c_str();
    inF.open(input, ios::binary);
    istream inS(&inF);

    //find beginning of stream
    inS.seekg(0, ios_base::beg);

    //create BitInputStream object
    BitInputStream in(inS);

    //read lines from header 
    vector<int> freqs(256, 0); //one slot per ascii value = 0
    unsigned int intMask = 0x1;
    unsigned int nextInt = 0;
    unsigned int totalChars = 0;
    for(int i = 0; i < 256; i++) {
        //get int
        intMask = 0x1;
        for(int j = 0; j < 32; ++j) {
            if(in.readBit()) {
                nextInt = intMask | nextInt;
            }
            intMask = intMask << 1;
        }
        //nextInt holds full int now
        freqs[i] = nextInt;
        totalChars += nextInt; //summing the freqs
    }

    //build tree
    HCTree tree;
    tree.build(freqs);

    //open out stream
    const char * output = outfile.c_str();
    ofstream out(output); //regular out
    if(!out.is_open()){
        cerr << outfile << " not opened!\n";
	    //return;
    }
    
    //decode each char 
    unsigned char nextChar; //for reading output
    for(unsigned int i = 0; i < totalChars; ++i) {
    	nextChar = tree.decode(in);
	    out << nextChar;
    }

    //close files
    if(out.is_open()){
    	out.close();
    }
    if(inF.is_open()){
    	inF.close();
    }

    //return;
}

int main(int argc, char ** argv) {
    string infile = "";
    string outfile = "";
    bool bitwise = false;
    for (int i = 1; i < argc; i++) {
        string currentArg = argv[i];
        if (currentArg == "-b") {
            bitwise = true;
        } else if (infile == "") {
            infile = currentArg;
        } else {
            outfile = currentArg;
        }
    }

    if (infile == "" || outfile == "") {
        cout << "ERROR: Must provide input and output files" << endl;
        print_usage(argv);
        return 1;
    }

    if (bitwise) {
        uncompressBitwise(infile, outfile);
    } else {
        uncompressAscii(infile, outfile);
    }

    return 0;
}
