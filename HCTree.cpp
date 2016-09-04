/*****************************P2 FILE HEADER********************************/
/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: HCTree.cpp
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

#include "HCTree.hpp"
#include <stack>

#define BYTE 8

/* root accessor method*/
HCNode* HCTree::getRoot(){
  return root;
}

/* getting the header bits*/
int HCTree::getHeaderBits(){
  return headerBits;
}

/* getting total characters*/
int HCTree::getTotalChars(){
  return totalChars;
}

/* determines whether or not node is a leaf*/
bool HCTree::isLeaf(HCNode* node){
  if((node->c0 == NULL) && (node -> c1 == NULL)){
    return true;
  }
  return false;
}


/* Preorder traversal helper method to fill in the tree
 * leaves vector*/
void HCTree::fillLeaves(HCNode* current, int index){
  if(current != NULL){

    if(isLeaf(current)){

      //record how many leaf bits we're writing to the header
      headerBits+=(BYTE+1);

      leaves[index] = current;
      fillLeavesIndex = fillLeavesIndex+1;
    }
    else{
      //record how many inner node bits we're writing to header
      headerBits++;
    }

    //go left
    fillLeaves(current->c0, fillLeavesIndex);
    //go right
    fillLeaves(current->c1, fillLeavesIndex);
    
  }
  return;
}


/* Prints out entire tree in postorder.
 * PRECONDITION: build() has been called */
void HCTree::printHeader(HCNode* current, BitOutputStream& out){

  //go left
  if(current->c0){ printHeader(current->c0, out); }
  //go right
  if(current->c1){ printHeader(current->c1, out); }

  //visit node

  //for a nonleaf store a 0 bit flag
  if(!isLeaf(current)){ 
    out.writeBit(0);
  }
  //for a leaf store bitflag1+ ASCII (9bits)
  else{
    out.writeBit(1);
    out.writeByte(current->symbol);
  }

}

/* helper method for tree destructor.
 * deallocates memory for all nodes in the tree*/
void HCTree::deleteAll(HCNode *&p){
    if(p == 0){
        return;
    }

    //go left
    deleteAll(p -> c0);
    //go right
    deleteAll(p -> c1);

    //visit and delete node
    delete p;
    p = NULL;
}

/* Destructor */
HCTree::~HCTree(){
    deleteAll(root);
}

/** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
void HCTree::build(const vector<int>& freqs){
    /* 1.create the forest of single node trees containing symbols
     * and counts for each non zero count symbol
     * */
    std::priority_queue<HCNode*, std::vector<HCNode*>,
                        HCNodePtrComp> pq; //to hold our sorted nodes

    int i;
    for(i = 0; i < (int)freqs.size(); i++){
      if(freqs[i] != 0){

        //record how many characters we're encoding
        totalChars += freqs[i];

        //create a new node to put in the queue
        HCNode * ptr = new HCNode(0, 0, 0, 0, 0);
        ptr -> count = freqs[i];
        ptr-> symbol = (unsigned char) i;
        pq.push(ptr);
      }

    }
    //condition for if there is only one character in entire file
    if(pq.size() == 1){  
      HCNode * singleParent = new HCNode(0, 0, 0, 0, 0);
      HCNode * singleChild = pq.top();
      pq.pop();
      singleParent-> symbol = singleChild->symbol;
      singleParent->count = singleChild->count;
      singleParent->c0 = singleChild;
      singleChild->p = singleParent;
      pq.push(singleParent);

    }


    /* 2. Loop
     *      
     * */
    while(pq.size() > 1){
        HCNode* parent = new HCNode(0, 0, 0, 0, 0);

        // Remove the two lowest count trees
        HCNode* child1 = pq.top();
        pq.pop();
        HCNode* child2 = pq.top();
        pq.pop();
        
        // Combine the two into a new tree
        parent -> count  = (child1->count)+(child2->count);
        parent -> symbol = child1->symbol;
        parent -> c0 = child1;
        parent -> c1 = child2;
        child1 -> p = parent;
        child2 -> p = parent;
        
        // Insert new tree into the forest
        pq.push(parent);
     }

    /* If there exists only one node in the priority queue,
     * then the root is going to be the only node.
     * */
    if(pq.size() == 1){
      root = pq.top();
    }
    fillLeaves(root, fillLeavesIndex);
    //reset the fillLeafIndex
    fillLeavesIndex = 0;

}

/** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
void HCTree::encode(byte symbol, BitOutputStream& out)const{
  stack<int> code; //holds coded value of character
  HCNode * leaf;

  //find the leaf
  for(int i = 0; i < (int)leaves.size(); i++){
    if(leaves[i]->symbol == symbol){
      //traverse from leaf to parent and store 0 or 1 in vector
      leaf = leaves[i];
      while( leaf-> p != NULL){
        if(leaf == leaf->p->c0){
          code.push(0);
        }
        else if(leaf == leaf->p->c1){
          code.push(1);
        }
        else{ break; }
        leaf = leaf->p;
      }
      break;
    }
  }
  
  int keeptrack = 0; //codebit to write to file

  //write the code to the file
  while(!code.empty()){
    keeptrack = code.top();
    out.writeBit(keeptrack);
    code.pop();
    
  }

}

/** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
int HCTree::decode(BitInputStream& in)const{
  HCNode * current = root;
  int bit = 0;

  //start at root and traverse tree until you reach a leaf
  while((current->c1 != NULL)&&(current->c0 != NULL)){

    //read in the bits
    bit = in.readBit();

    if(bit == 0){
      //don't read the extra zeroes padding
      if(totalChars <= 0){
        continue;
      }
      else{
        current = current -> c0;
      }
    }
    else if(bit == 1){
      if(totalChars <=0){
        continue;
      }
      current = current -> c1;
    }
    else{ return -1;}
  }
  //keep track of how many characters you've already read
  totalChars--;
  //return the symbol in the leaf node
  return current-> symbol;
    
}

void HCTree::rebuild(BitInputStream& in){
  int bitRead;
  int byteRead;
  int headerBits = 0;
  stack<HCNode*> nodes;


  //read the header
  totalChars = in.readInt();
  headerBits = in.readInt();

  while(headerBits != 0){

    bitRead = in.readBit();
    headerBits--;  

    if(bitRead ==1){
      //node is a leaf so read the ASCII
      byteRead = in.readByte();
      headerBits-= BYTE;
      //create a node
      HCNode * node = new HCNode(0, byteRead, 0, 0, 0);
      //push it to the stack
      nodes.push(node);
    }
    else{
      //node is an inner node
      //pop two nodes from the stack
      if(!nodes.empty()){
        HCNode*right = 0;

        //condition to handle single character case
        if(nodes.size() >1){
          right = nodes.top();
          nodes.pop();
        }

        HCNode* left = nodes.top();
        nodes.pop();

        //link the nodes
        HCNode* parent = new HCNode(0, 0, 0, 0, 0);
        parent->symbol = left->symbol;
        parent ->c0 = left;
        left ->p = parent;

        //condition to handle single character case
        if(right != NULL){
          right -> p = parent;
          parent -> c1 = right;
        }

        //push new parent back in
        nodes.push(parent);
      }
    }
  }
  
  root = nodes.top();

  //handle single character case
  if(root->c1 == NULL){
    HCNode* dummyNode = new HCNode(0, 0, 0, 0, 0);
    dummyNode->symbol = root->symbol;
    dummyNode->p = root;
    root->c1 = dummyNode;
  }

}

