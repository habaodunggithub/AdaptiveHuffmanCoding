#include<iostream>
#include <fstream>
#include "AdaptiveHuffmanCoding.h"
int main() {
	AdaptiveHuffmanCoding encoder;
	encoder.inpFile = "test.txt";
    encoder.outFile = "code.txt";
    std::ifstream inputFile(encoder.inpFile);
    std::ofstream outputFile(encoder.outFile, std::ios::binary);
    uint64_t bitBuffer = 0;
    int bitCount = 0;
	char symbol;

	while (inputFile.get(symbol)) {
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
	}

    int code = 0;
    int len = 0;
	encoder.Encode(AdaptiveHuffmanCoding::PSEUDO_EOF, code, len);
    bitBuffer <<= len;
    bitBuffer |= code;
    bitCount += len;

    while (bitCount >= 8) {
        uint8_t byte = (bitBuffer >> (bitCount - 8)) & 0xFF;
        outputFile.put(byte);
        bitCount -= 8;
    }
    if (bitCount > 0) {
        uint8_t byte = (bitBuffer << (8 - bitCount)) & 0xFF;
        outputFile.put(byte);
    }
    inputFile.close();
    outputFile.close();

    AdaptiveHuffmanCoding decoder;
    decoder.inpFile = "code.txt";
    decoder.outFile = "decode.txt";

    decoder.Decode();


    return 0;
}