#include <stack>
#include <queue>
#include <algorithm>

#include "HCTree.hpp"

/**
 * Destructor for HCTree
 */
HCTree::~HCTree() {
    if(root->c0 == nullptr) {
        deleteTree(root->c0);
    }
    if(root->c1 == nullptr) {
        deleteTree(root->c1);
    }
    delete(root);
    return;
}

/**
 * helper for recursive deletion of nodes in tree
 */
void deleteTree(HCNode * subroot) {
    if(subroot->c0 == nullptr) {
        deleteTree(root->c0);
    }
    if(subroot->c1 == nullptr) {
        deleteTree(root->c1);
    }
    delete(subroot);
    return;
}

/** Use the Huffman algorithm to build a Huffman coding tree.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the tree,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
    if(root != nullptr) {
        deleteTree(root);
    }

    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pqueue;

    //create leaf node for each symbol
    int forestCount = 0;
    for(int i = 0; i < freqs.size(); i++) {
        if(freqs[i] > 0) {
            HCNode * tmp = new HCNode(freqs[i], (byte)i);
            leaves[i] = tmp;
            pqueue.push(tmp);
            forestCount++;
        }
    }

    //get top 2 leaf nodes from pqueue to make subtree
    HCNode * left;
    HCNode * right;
    HCNode * parent;
    int pCount = 0;
    byte newSym = 0;
    for(int i = forestCount; i > 1; --i) {
        //save pop first 2 from pqueue
        left = pqueue.top();
        pqueue.pop();
        right = pqueue.top();
        pqueue.pop();

        //create new parent node
        pCount = left->count + right->count;
        if(left.symbol < right.symbol) {
            newSym = right.symbol;
        }
        else {
            newSym = left.symbol;
        }
        parent = new HCNode(pCount, newSym, left, right);
        left->p = parent;
        right->p = parent;

        //add back into pqueue
        pqueue.push(parent);
    }
    root = parent;

    return;
}

/**
 * creates vector where index is char and value at index 
 * is a string of 0 and 1's 
 */
void getEncodings() {
    HCNode * leaf;
    for(int i = 0; i < 256; ++i) {
        if(leaves[i] != nullptr) {
            leaf = leaves[i];
            string path = getPath(leaf, nullptr);
            encodings[i] = new string(path);
        }
    }
}

/**
 * takes the leaf node and finds the path up to the root 
 * keeping track of the path and then reversing it
 */
string getPath(HCNode * leaf, HCNode * prev) {
    string path = nullptr;
    if(leaf->parent != nullptr) {
        path = getPath(leaf->parent, leaf);
    }
    if(leaf->left == prev) {
        //careful base case size 1 and 0 tree
        return '0' + path;
    }
    else {
        return '1' + path;
    }
}


/** Write to the given ostream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    // TODO (checkpoint)
    string encodedSymbol;
    if(leaves[(int)symbol] == nullptr) {
        //error symbol not found in file
        //      or something wrong with leaves
        return -1;
    }
    encodedSymbol = encodings[(int) symbol];
    out << encodedSymbol;
    return;
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the istream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
byte HCTree::decode(istream& in) const {
    //TODO (checkpoint)
    //how to delete from front in istream
    //traverse tree until istream is empty
    //  when reach leafnode 
    //     write char and 
    //     set durr to root
    return 0;
}



/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    // TODO (final)
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
byte HCTree::decode(BitInputStream& in) const {
    return 0;  // TODO (final)
}

/**
 * Print the contents of a tree
 */
void HCTree::printTree() const {
    cout << "=== PRINT TREE BEGIN ===" << endl;
    printTreeHelper(root);
    cout << "=== PRINT TREE END =====" << endl;
}

/**
 * Recursive helper function for printTree
 */
void HCTree::printTreeHelper(HCNode * node, string indent) const {
    if (node == nullptr) {
        cout << indent << "nullptr" << endl;
        return;
    }

    cout << indent << *node << endl;
    if (node->c0 != nullptr || node->c1 != nullptr) {
        printTreeHelper(node->c0, indent + "  ");
        printTreeHelper(node->c1, indent + "  ");
    }
}
