#pragma once

#include <string>

class AdaptiveHuffmanCoding {
private:
	std::string inpFile, outFile;

	struct HuffmanNode {
		int value;    
		int weight;   
		int order;   

		HuffmanNode* left;
		HuffmanNode* right;
		HuffmanNode* parent;

		bool isNYT;

		HuffmanNode(int value, int weight, int order, HuffmanNode* left, HuffmanNode* right,
			HuffmanNode* parent, bool isNYT = false)
			:value(value), weight(weight), order(order), left(left), right(right), parent(parent), isNYT(isNYT)
		{}
		HuffmanNode()
			: value(0), weight(0), order(0), left(nullptr), right(nullptr), parent(nullptr), isNYT(false)
		{}

		HuffmanNode(bool isNYT, int value)
			: HuffmanNode()
		{
			this->isNYT = isNYT;
			this->value = value;
		}
	};

	HuffmanNode* GetSymbolNode(int, HuffmanNode*) const;
	void FindBlockLeader(HuffmanNode*, HuffmanNode*&) const;
	void SwapNodes(HuffmanNode*, HuffmanNode*);
	void GetPathToSymbol(HuffmanNode*, HuffmanNode*, int& code, int& len, int curCode, int curLen) const;
	void SlideAndIncrement(HuffmanNode*);

	void DeleteHuffmanTree(HuffmanNode*);

	HuffmanNode	*NYTNode;
	HuffmanNode *root;
    
public:
	AdaptiveHuffmanCoding();
	void UpdateTreeModel(int);
	void Encode(int, int&, int&);
	
	~AdaptiveHuffmanCoding();
	static const int PSEUDO_EOF = 256;
};
