#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitInputStream.hpp"

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
    cerr << "TODO: uncompress '" << infile << "' -> '"
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

    //TODO check for empty file
    in.seekg(0, ios_base::end);
    
    }

     //find beginning of stream
    in.seekg(0, ios_base::beg);

    //read lines from stream
    vector<int> freqs(256, 0); //one slot per ascii value = 0
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

    //get to beginning again
    //open out stream
    const char * output = outfile.c_str();
    ofstream out(output);
    if(!out.is_open()){
        cout << outfile << " not opened!\n";
	return;
    }
    //output header
    for (int i = 0; i < 256; ++i){
    	 out << freqs[i] << en
    }

    if(in.is_open()){
    	in.close();
    }

    in.open(input, ios::binary); //reopen input

    in.seekg(0, ios_base::beg);

    while((nextByte = in.get()) != EOF) {
    	nextChar = (unsigned char) nextByte;
	tree.encode(nextChar, out);
    }
    
    //close files
    if(out.is_open()){
    	out.close();
    }

    if(in.is_open()){
    	in.close();
    }

    return;
}


/**
 * Reads infile, constructs a Huffman coding tree based on its header,
 * and produces an uncompressed version in outfile.
 * Uses bitwise I/O.
 */
void uncompressBitwise(const string & infile, const string & outfile) {
    // TODO (final)
    cerr << "TODO: uncompress '" << infile << "' -> '"
        << outfile << "' here (bitwise)" << endl;
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
