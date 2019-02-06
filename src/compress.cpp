#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitOutputStream.hpp"

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
    in.open(infile.c_str(), ios::binary);

    //check if file actually opened 
    if(!in.is_open()) {
        cout << "input file was not opened...\n";
        return -1;
    }

    //check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if(len == 0) {
        cout << "input file opened but empty\n";
        return -1;
    }

    //find beginning of stream
    in.seekg(0, ios_base::beg);
    
    //read lines from stream
    string line;
    vector<int> freqs(256, 0); //one slot per ascii value = 0
    while(getine(in, line)) {
        for(char& ch : line) {
            freqs[(int)ch]++;
        }
    }

    //build tree
    HCTree tree;
    tree.build(freqs);
    tree.getEncodings();
    
    //get to beginning again
    in.seekg(0, ios_base::beg);

    //open out stream
    ofstream out(outfile.c_str());
    if(!out.is_open()) {
        cout << outfile << " not opened!\n";
        return -1;
    }

    while(getline(in, line)) {
        for(char & ch : line) {
            out << 
  
        }
    }
    
    //close file
    if(in.is_open()) {
        in.close();
    }

    return 0;
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
