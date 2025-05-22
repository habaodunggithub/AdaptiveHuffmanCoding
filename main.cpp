#include<iostream>
#include <fstream>
#include "AdaptiveHuffmanCoding.h"

int main(int argc, char** argv) {   
    std::string query(argv[1]);
    
    if (query == "--encode") {
        AdaptiveHuffmanCoding encoder;
        encoder.inpFile = argv[2];
        encoder.outFile = argv[3];
        encoder.Encode();
    }
    else if (query == "--decode") {
        AdaptiveHuffmanCoding decoder;
        decoder.inpFile = argv[2];
        decoder.outFile = argv[3];
        decoder.Decode();
    }
    else
        std::cout << "Invalid query!\n";
    return 0;
}