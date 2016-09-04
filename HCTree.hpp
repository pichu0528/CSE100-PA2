/*****************************P2 FILE HEADER********************************/
/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: HCTree.hpp
* Date: 22 April 2015
*
* Description:
* In HCTree class, we implement build function to build a Huffman
* coding tree. That way we can build the tree in our compress.cpp
* and uncompress.cpp. Along with build() and rebuild(), we also have 
* encode() and decode(). We use encode() to encode the input file into 
* bits for compress.cpp, and decode() to decode the results we have from
* compress.cpp to translate back to the original input file.
**/
/***************************************************************************/

#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};


/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;
    int fillLeavesIndex = 0; //used to increment index in fillLeaves
   
    int headerBits; //amount of bits encoded as header
    mutable int totalChars; //amount of characters encoded

 
   // Helper method to build/encode tree
    void fillLeaves(HCNode* current, int index);

    //helper for destructor
    void deleteAll(HCNode *&p);

    //determines whether or not node is a leaf
    bool isLeaf(HCNode * node);
   


public:
    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
        totalChars = 0;
        headerBits = 0;
    }

    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** REBUILD THE HUFFMAN TREE WITHOUT FREQUENCY
     */
    void rebuild(BitInputStream& in);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Methods to rebuild tree in uncompress
     *  Precondition: build() is called
     */
    void printHeader(HCNode* current, BitOutputStream& out);

    /* accessor method to get the root node outside of 
     * the tree class
     * */
    HCNode * getRoot();
    int getHeaderBits();
    int getTotalChars();

      

};

#endif // HCTREE_HPP
