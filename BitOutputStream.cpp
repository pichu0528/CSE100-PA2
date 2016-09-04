/*****************************P2 FILE HEADER********************************/
/* Name: Pin Chu A98041513, Jessica Tran A11358012
* Instructor: Debashis Sahoo
* Filename: BitOutputStream.cpp
* Date: 22 April 2015
*
* Description:
* This class initialize a BitOutPutStream object that can be
* performed with flush() and writeBit(int i ). This class is required
* for compress.cpp to test bits, instead of ASCII texts.
*
**/
/***************************************************************************/

#include "BitOutputStream.hpp"

#define BYTE 8

using namespace std;

  /** Write the least significant bit of the argument into
   *  the bit buffer, and increment the bit buffer index.
   *  Flush to the ostream first if the bit buffer is full.
   *  This must be consistent with BitInputStream::readBit().
   */
  void BitOutputStream::writeBit(int bit){
    //flush to ostream if bit buffer is full
    if(bufi == BYTE){
      flush();
    }

    //write LSB into buffer starting from the back
    int lsb = bit & 1;
    lsb = lsb << bufi;
    buf = lsb|buf;

    //increment the index
    bufi++;

  }

  /** Write the least significant byte of the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing bytes.
   */
  void BitOutputStream::writeByte(int b){
    int count =0;

    while(count < BYTE){
      //write one bit ( the lsb of b)
      writeBit(b);

      //shift b to get the next lsb
      b = b >> 1;

      count++;
    }
  }


  /** Write the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing ints.
   */
  void BitOutputStream::writeInt(int i){
    int count =0;

    while(count < (int)sizeof(int)*BYTE){

      //write one bit
      writeBit(i);

      //shift i to get the next lsb
      i = i >> 1;
      count++;
    }
  }

  /** If the bit buffer contains any bits, flush the bit buffer to the ostream,
   *  clear the bit buffer, and set the bit buffer index to 0.
   *  Also flush the ostream itself.
   */
  void BitOutputStream::flush(){
    out.put(buf);
    out.flush();
    buf= '\0';
    bufi=0;
  }
  
 
