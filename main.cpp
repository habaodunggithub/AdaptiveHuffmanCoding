#include<iostream>
#include <fstream>
#include "AdaptiveHuffmanCoding.h"
int main() {
	AdaptiveHuffmanCoding encoder;
	encoder.inpFile = "test.txt";
    encoder.outFile = "code.txt";
    encoder.Encode();
    
    AdaptiveHuffmanCoding decoder;
    decoder.inpFile = "code.txt";
    decoder.outFile = "decode.txt";
    
    decoder.Decode();

    return 0;
}