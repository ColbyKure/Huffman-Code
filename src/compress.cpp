#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitOutputStream.hpp"

#define MAX_CHAR 256

using namespace std;

void print_usage(char ** argv) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " INFILE OUTFILE [-b]" << endl;
    cout << "Command-line flags:" << endl;
    cout << "  -b: switch to bitwise mode" << endl;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its contents,
 * and produces a compressed version in outfile.
 * For debugging purposes, uses ASCII '0' and '1' rather than bitwise I/O.
 */
void compressAscii(const string & infile, const string & outfile) {
    //open file 
    ifstream in;
    const char * input = infile.c_str();
    in.open(input, ios::binary);

    //check if file actually opened 
    if(!in.is_open()) {
        cerr << "input file was not opened...\n";
        //return;
    }

    //check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if(len == 0) {
        cerr << "input file opened but empty\n";
        //return;
    }

    //find beginning of stream
    in.seekg(0, ios_base::beg);

    //get freqs from file
    vector<int> freqs(MAX_CHAR, 0); //one slot per ascii value = 0
    unsigned char nextChar;
    int nextByte, index;
    while((nextByte = in.get()) != EOF) {
        nextChar = (unsigned char) nextByte;
        index = (int)nextChar;
        freqs[index] = freqs[index] + 1;
    }

    //build tree
    HCTree tree;
    tree.build(freqs);
    
    //open out stream
    const char * output = outfile.c_str();
    ofstream out(output);
    if(!out.is_open()) {
        cerr << outfile << " not opened!\n";
        //return;
    }
    //output header
    for(int i = 0; i < MAX_CHAR; ++i) {
        out << freqs[i] << endl;
    }

    //close in stream
    if(in.is_open()) {
        in.close();
    }
    
    //reopen in stream
    in.open(input, ios::binary); //reopen input

    //seek beginning again
    in.seekg(0, ios_base::beg);

    //encode the file
    while((nextByte = in.get()) != EOF) { //read infile again
        nextChar = (unsigned char) nextByte;
        tree.encode(nextChar, out);
    }
    
    //close files
    if(out.is_open()) {
        out.close();
    }
    if(in.is_open()) {
        in.close();
    }

    //return;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its contents,
 * and produces a compressed version in outfile.
 * Uses bitwise I/O.
 */
void compressBitwise(const string & infile, const string & outfile) {
    //open file 
    ifstream in;
    const char * input = infile.c_str();
    in.open(input, ios::binary);

    //check if file actually opened 
    if(!in.is_open()) {
        cerr << "input file was not opened...\n";
        //return;
    }

    //check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if(len == 0) {
        cerr << "input file opened but empty\n";
        //return;
    }

    //find beginning of stream
    in.seekg(0, ios_base::beg);

    //get freqs
    vector<int> freqs(256, 0); 
    unsigned char nextChar;
    int nextByte, index;
    while((nextByte = in.get()) != EOF) {
        nextChar = (unsigned char) nextByte;
        index = (int)nextChar;
        freqs[index] = freqs[index] + 1;
    }

    //build tree
    HCTree tree;
    tree.build(freqs);
    
    //open out stream
    const char * output = outfile.c_str();
    ofstream outBuf(output, ios::binary);
    // outBuf.open(output, ios_base::binary); 
    // outBuf << "THIS IS A TEST!!!@ PLS WRITE\n\n\n\n\n\n";
    // ostream casted = (ostream)outBuf;
    
    // ostream outS(&outBuf);
    BitOutputStream out(outBuf);
    
    //output header
    unsigned int currentFreqs, tmp;
    unsigned int mask = 0x1;

    // SANITY TEST
    /* for (int i=0; i < 256; i++) {
        cout << "sanity test of freq: " << freqs[i] << "\n";
    } */



    for(int i = 0; i < 256; ++i) {
	    currentFreqs = freqs[i]; ///get freq to encode
cout << "current frequency is: " << hex << currentFreqs << endl;
        mask = 0x1 << 31; //init mask

        //for each bit in int write a bit
	    for (int k = 0; k < 32; ++k){
            out.writeBit(currentFreqs & mask);
            mask = mask >> 1;
            //cout << "current mask is: " << hex << mask << endl;
        }
	        
    }//done with header

    //close in stream
    if(in.is_open()) {
        in.close();
    }

    //reopen in stream
    in.open(input, ios_base::binary);

    //fdin beginning of file
    in.seekg(0, ios_base::beg);

    //encode each char (bitwise)
    while((nextByte = in.get()) != EOF) { //read infile again
        nextChar = (unsigned char) nextByte;
        tree.encode(nextChar, out);
    }
    
    //close files
    if(in.is_open()) {
        in.close();
    }
    if(outBuf.is_open()) {
        outBuf.close();
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
        compressBitwise(infile, outfile);
    } else {
        compressAscii(infile, outfile);
    }

    return 0; //EXIT_SUCCESS
}
