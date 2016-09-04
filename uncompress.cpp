/*****************************P2 FILE HEADER********************************/
/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: uncompress.cpp
* Date: 22 April 2015
*
* Description:
* Main function to uncompress the encoded files from the command to an
* output file with decoded data. And by uncompressing, we need to go
* thru:
*
* 1. Open the input file for reading.
* 2. Read the file header at the beginning of the input file, and
*    reconstruct the Huffman coding tree.
* 3. Open the output file for writing.
* 4. Using the Huffman coding tree, decode the bits from the input file
*    into the appropriate sequence of bytes, writing them to the output
*    file.
* 5. Close the input and output files.
*
**/
/***********************************************************************/

#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"
#include <iostream>

using namespace std;

int main(int argc, char ** argv){
  ifstream in;  //stream we're reading in from
  ofstream out; //stream to output to
  BitInputStream input(in);
  BitOutputStream output(out);

  //check for correct number of arguments
  if(argc != 3){ 
    cerr << "Incorrect number of arguments \n";
    return -1;
  }

  string infile = argv[1];
  string outfile = argv[2];

  //open the infile
  in.open(infile.c_str(), ios::binary);

  //open the output file for writing
  out.open(outfile.c_str(), ios::binary);

  //check if the file is empty
  if (in.peek() == std::ifstream::traits_type::eof()){
    in.close();
    out.close();

    return 1;

  } 

  //rebuild the tree
  HCTree* tree = new HCTree();
  tree->rebuild(input);

  int decodedSymb = 0; //return value from decode

  //decode the encoded bits
  while(1){
    decodedSymb = tree->decode(input);
    
    //reading has reached end of file
    if(decodedSymb == -1){ break; }

    output.writeByte(decodedSymb);
  }
  
  output.flush();
  //check that entire file was read
  if(!in.eof()){
    cerr << "There was a problem. Process exited early \n";
    return -1;
  }

  //close the streams
  in.close();
  out.close();

  //deallocate memory
  delete tree;
  tree = 0;

  return 1;

}
