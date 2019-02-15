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
    cerr << "TODO: compress '" << infile << "' -> '"
        << outfile << "' here (ASCII)" << endl;
    //open file 
    ifstream in;
    const char * input = infile.c_str();
    in.open(input, ios::binary);

    //check if file actually opened 
    if(!in.is_open()) {
        cout << "input file was not opened...\n";
        return;
    }

    //check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if(len == 0) {
        cout << "input file opened but empty\n";
        return;
    }

    //find beginning of stream
    in.seekg(0, ios_base::beg);

    //read lines from stream
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
        cout << outfile << " not opened!\n";
        return;
    }
    //output header
    for(int i = 0; i < MAX_CHAR; ++i) {
        out << freqs[i] << endl;
    }

    if(in.is_open()) {
        in.close();
    }

    in.open(input, ios::binary); //reopen input

    in.seekg(0, ios_base::beg);

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

    return;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its contents,
 * and produces a compressed version in outfile.
 * Uses bitwise I/O.
 */
void compressBitwise(const string & infile, const string & outfile) {
    // TODO (final)
    cerr << "TODO: compress '" << infile << "' -> '"
        << outfile << "' here (bitwise)" << endl;
    //open file 
    ifstream in;
    const char * input = infile.c_str();
    in.open(input, ios::binary);

    //check if file actually opened 
    if(!in.is_open()) {
        cout << "input file was not opened...\n";
        return;
    }

    //check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if(len == 0) {
        cout << "input file opened but empty\n";
        return;
    }

    //find beginning of stream
    in.seekg(0, ios_base::beg);

    //read lines from stream
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
    ostream outS(output);
    BitOutputStream * out(outS);
    
    //output header
    unsigned int currentFreqs;
    unsigned int mask = 0x1;
    mask = mask << 31;
    for(int i = 0; i < 256; ++i) {
	currentFreqs = freqs[i]; 
	for (int k = 0; k < 32; ++k){
            currentFreqs = currentFreqs & mask;
	    if (currentFreqs != 0){ //if the bit is one
	    	out.writeBit(1);
	    }
	    else{
	    	out.writeBit(0);
	    } 
	}
    }

    if(in.is_open()) {
        in.close();
    }

    in.seekg(0, ios_base::beg);

    while((nextByte = in.get()) != EOF) { //read infile again
        nextChar = (unsigned char) nextByte;
        tree.encode(nextChar, out);
    }
    
    //close files
    if(in.is_open()) {
        in.close();
    }
    
    ofstream out(output);
    if(!out.is_open()) {
        cout << outfile << " not opened!\n";
        return;
    }
    //output header
    for(int i = 0; i < 256; ++i) {
        out << freqs[i] << endl;
    }

    if(in.is_open()) {
        in.close();
    }
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

    return 0;
}
