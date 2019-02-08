#include <stack>
#include <queue>
#include <algorithm>

#include "HCTree.hpp"

/**
 * Destructor for HCTree
 */
HCTree::~HCTree() {
    if(root == nullptr) {
        return;
    }

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
void HCTree::deleteTree(HCNode * subroot) {
    if(subroot->c0 != nullptr) {
        deleteTree(subroot->c0);
    }
    if(subroot->c1 != nullptr) {
        deleteTree(subroot->c1);
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
    for(unsigned int i = 0; i < freqs.size(); i++) {
        if(freqs[i] > 0) {
            HCNode * tmp = new HCNode(freqs[i], (byte)i);
            leaves[i] = tmp;
            pqueue.push(tmp);
            forestCount++;
        }
    }
    if(forestCount == 0) {
        root = nullptr;
    }

    //get top 2 leaf nodes from pqueue to make subtree
    HCNode * left;
    HCNode * right;
    HCNode * parent;
    int pCount = 0; //parent count
    byte newSym = 0;
    for(int i = forestCount; i > 1; --i) {
        //save pop first 2 from pqueue
        left = pqueue.top();
        pqueue.pop();
        right = pqueue.top();
        pqueue.pop();

        //create new parent node
        pCount = left->count + right->count;
        if(left->symbol < right->symbol) {
            newSym = right->symbol;
        }
        else {
            newSym = left->symbol;
        }
        parent = new HCNode(pCount, newSym, left, right);
        left->p = parent;
        right->p = parent;

        //add back into pqueue
        pqueue.push(parent);
    }
    root = parent;

    getEncodings();
    return;
}

/**
 * creates vector where index is char and value at index 
 * is a string of 0 and 1's 
 */
void HCTree::getEncodings() {
    HCNode * leaf;
    for(int i = 0; i < 256; ++i) {
        if(leaves[i] != nullptr) {
            leaf = leaves[i];
            string path = getPath(leaf, nullptr);
            encodings[i] = path.c_str();
        }
    }
}

/**
 * takes the leaf node and finds the path up to the root 
 * keeping track of the path and then reversing it
 */
string HCTree::getPath(HCNode * leaf, HCNode * prev) {
    string path;
    if(leaf->p != nullptr) {
        path = getPath(leaf->p, leaf);
    }
    if(prev == nullptr) {
        return path;
    }
    if(leaf->c0 == prev) {
        //careful base case size 1 and 0 tree
        return path + '0';
    }
    else {
        return path + '1';
    }
}

/** Write to the given ostream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    string encodedSymbol;
    if(leaves[(int)symbol] == nullptr) {
        cout << "did not find symbol\n";
        return;
    }
    encodedSymbol = encodings[(int)symbol];
    out << encodedSymbol;
    return;
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the istream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
byte HCTree::decode(istream& in) const {
    HCNode * curr = root;
    unsigned char nextChar;
    in >> nextChar;
    while(true) {
        if(nextChar == '0') {
            curr = curr->c0;
        }
        else {
            curr = curr->c1;
        }
        if(curr->c0 == nullptr) {
            //curr must be leaf node
            return curr->symbol;
        }
        if(in.eof()) {
            break;
        }
        in >> nextChar;
    }
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
