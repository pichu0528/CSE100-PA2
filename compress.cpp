/*****************************P2 FILE HEADER********************************/
/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: compress.cpp
* Date: 22 April 2015
*
* Description:
* Main function to compress the input files from the command to an
* output file with encoded bits. And by compressing, we need to go thru:
*
* 1. Open the input file for reading.
* 2. Read bytes from the file, counting the number of occurrences of each
* byte value; then close the file.
* 3. Use these byte counts to construct a Huffman coding tree.
* 4. Open the output file for writing.
* 5. Write enough information (a "file header") to the output file to
* enable the coding tree to be reconstructed when the file is read by
* your uncompress program.
* 6. Open the input file for reading, again.
* 7. Using the Huffman coding tree, translate each byte from the input
* file into its code, and append these codes as a sequence of bits to
* the output file, after the header.
* 8. Close the input and output files.
*
**/
/***************************************************************************/

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv){
  ifstream in;
  ofstream out;
  HCNode* root;
  int totalChars;
  int headerBits;
  
  if(argc != 3){
    cerr << "Invalid argument" << endl;
    return -1;
  }

  string filename1 = argv[1]; //infile
  string filename2 = argv[2]; //outfile

  //open the first file
  in.open(filename1.c_str(), ios::binary);

  vector<int> frequency(256, 0);
  int character = 0;

  // Opening the output file for writing
  out.open(filename2.c_str(), ios::binary);
  BitOutputStream output(out);

  //check if the file is empty
  if (in.peek() == std::ifstream::traits_type::eof()){
    //do nothing to the file
    in.close();
    out.close();

    return 1;

  }
 
  //count the frequencies of each character in the file
  while(1){
    character = in.get();
    if(!in.good()) { break;}
    frequency[character] = frequency[character] +1;
  }

  //check that entire file was read
  if(!in.eof()){
    cerr << "There was a problem, sorry." << endl;
    return -1;
  }

  // Closing the input file
  in.close();

  // Build the Huffman tree
  HCTree* tree = new HCTree();
  tree->build(frequency);

  in.open(filename1.c_str(), ios::binary);

  //print how many chars are encoded
  totalChars = tree->getTotalChars();
  output.writeInt(totalChars);

  //print how long the header is
  headerBits = tree->getHeaderBits();
  output.writeInt(headerBits);

  //print the header for info to rebuild the tree
  root = tree->getRoot();
  tree->printHeader(root, output);

  
  byte letter = 0;

  // Translate each byte from the input file into its code, and then write 
  // it to output file
  while(1){
    letter = in.get();
    if(!in.good()){break;}
    tree->encode(letter, output); 
  }

  //flush the remaining bits even if buffer not full
  output.flush();

  //check that entire file was read
  if(!in.eof()){
    cerr << "There was a problem, sorry" << endl;
    return -1;
  }

  // Close the input and output files
  in.close();
  out.close();

  //deallocate memory
  delete tree;
  tree = 0;

  return 1;
}


