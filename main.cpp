#include<iostream>
#include <fstream>
#include "AdaptiveHuffmanCoding.h"
#include <cstdint>
int main() {
    std::ifstream inputFile("test.txt");
	
	AdaptiveHuffmanCoding encoder;
	std::ofstream outputFile("output.txt", std::ios::binary);
	char symbol = inputFile.get();
    uint64_t bitBuffer = 0;
    int bitCount = 0;

	while (!inputFile.eof()) {
        int code = 0;
        int len = 0;
        encoder.Encode(symbol, code, len);

        bitBuffer <<= len;
        bitBuffer |= code;
        bitCount += len;

        while (bitCount >= 8) {
            uint8_t byte = (bitBuffer >> (bitCount - 8)) & 0xFF;
            outputFile.put(byte);
            bitCount -= 8;
        }
		
		symbol = inputFile.get();
	}

    if (bitCount > 0) {
        uint8_t byte = (bitBuffer << (8 - bitCount)) & 0xFF;
        outputFile.put(byte);
    }

    // int code = 0;
    // int len = 0;
	// encoder.Encode(AdaptiveHuffmanCoding::PSEUDO_EOF, code, len);
    // while (bitCount >= 8) {
    //     uint8_t byte = (bitBuffer >> (bitCount - 8)) & 0xFF;
    //     outputFile.put(byte);
    //     bitCount -= 8;
    // }
    // if (bitCount > 0) {
    //     uint8_t byte = (bitBuffer << (8 - bitCount)) & 0xFF;
    //     outputFile.put(byte);
    // }

    inputFile.close();
    outputFile.close();
    return 0;
}