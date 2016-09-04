/*****************************P2 FILE HEADER********************************/
/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: BitInputStream.cpp
* Date: 22 April 2015
*
* Description:
* This class initialize a BitInPutStream object that can be
* performed with fill() and readBit().This class is required for
* uncompress.cpp to test bits, instead of ASCII texts.
*
**/
/***************************************************************************/

#include "BitInputStream.hpp"    
#define BYTE 8

     /** Fill the buffer from the input */
     void BitInputStream::fill(){
         buf  = in.get();
         bufi = 0;
     }
     
   /** Read the next bit from the bit buffer.
     *  If the bit buffer is currently empty,
     *  fill the bit buffer by reading a char from the istream first.
     *  Return the bit read as the least signficant bit of an int.
     *  Return -1 on EOF.
     *  This must be consistent with BitOutputStream::writeBit(), in terms
     *  of ordering of bits in the stream.
     */
    int BitInputStream::readBit(){
      // Return -1 on EOF
      if(!in.good()){
          return -1;
      }

      // Refill the buffer if it is full
      if(bufi == BYTE){ 
          fill();
      }

      // Return the LSB.
      int lsb = 0;
      lsb = buf & 1;
      bufi++;

      //shift the buffer to prep for the next lsb
      buf = buf >> 1;

      return lsb;
    
    }

    /** Read a byte from the ostream(istream?).
     *  Return -1 on EOF.
     *  This function doesn't touch the bit buffer.
     *  The client has to manage interaction between reading bits
     *  and reading bytes.
     */
    int BitInputStream::readByte(){
      // Return -1 on EOF
      if(!in.good()){
          return -1;
      }

      int count = 0; //how many bits we've read

      int tempBit = 0; //bit read in
      int result = 0; //return value

      while(count < BYTE){
          //get the bit
          tempBit = readBit();
          //shift the bit left to the correct index
          tempBit = tempBit << count;

          //combine the current bit with the return value
          result = tempBit | result;
          count++;
      }
      return result;
    }

    /** Read a non-negative int from the ostream(istream?).
     *  Return -1 on EOF.
     *  This function doesn't touch the bit buffer.
     *  The client has to manage interaction between reading bits
     *  and reading ints.
     */
    int BitInputStream::readInt(){
      // Return -1 on EOF
      if(!in.good()){
          return -1;
      }

      int count = 0;
      int tempBit = 0;
      int result = 0;

      while(count < ((int)sizeof(int)*BYTE)){
          //get the bit
          tempBit = readBit();
          //shift the bit left to the correct value
          tempBit = tempBit << count;
          
          //combine the current bit with the previous one
          result = tempBit | result;
          count++;
      }
      return result;
    }

