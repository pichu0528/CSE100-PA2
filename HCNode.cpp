/*****************************P2 FILE HEADER********************************/

/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: HCNode.cpp
* Date: 22 April 2015
*
* Description:
* Class HCNode initializes a node for Huffman coding tree to build.
* And, we also implement the less-than comparison for the use in
* priority queue.
*
**/
/***************************************************************************/

#include "HCNode.hpp"

bool HCNode::operator<(const HCNode& other){
    if(count!=other.count){
        return count > other.count;
    }
    else{
        return symbol> other.symbol;
    }
}

