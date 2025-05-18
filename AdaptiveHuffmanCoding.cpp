#include "AdaptiveHuffmanCoding.h"


AdaptiveHuffmanCoding::AdaptiveHuffmanCoding() {
	root = new HuffmanNode(-1, 0, 512, nullptr, nullptr, nullptr, true);
    NYTNode = root;
}

void AdaptiveHuffmanCoding::UpdateTreeModel(int symbol) {
	HuffmanNode *symbolNode = GetSymbolNode(symbol, root);

	if (symbolNode == nullptr) {
		NYTNode->isNYT = false;
		int currentMinOrder = NYTNode->order;
		NYTNode->left = new HuffmanNode(-1, 0, currentMinOrder - 2, nullptr, nullptr, NYTNode, true);
		NYTNode->right = new HuffmanNode(symbol, 0, currentMinOrder - 1, nullptr, nullptr, NYTNode, false);
		symbolNode = NYTNode->right;
		NYTNode = NYTNode->left;
	}
			
	SlideAndIncrement(symbolNode);	
}

void AdaptiveHuffmanCoding::Encode(int symbol, int& code, int& len) {
	HuffmanNode *symbolNode = GetSymbolNode(symbol, root);
	if (symbolNode != nullptr) {
		GetPathToSymbol(root, symbolNode, code, len, 0, 0);
		UpdateTreeModel(symbol);
		return;
	}
	
	GetPathToSymbol(root, NYTNode, code, len, 0, 0);
	code = (code << 8) | static_cast<unsigned char>(symbol);
	len += 8;

	UpdateTreeModel(symbol);
}

void AdaptiveHuffmanCoding::Decode() {
	std::ifstream fin(inpFile, std::ios::binary);
	std::ofstream fout(outFile);
	if (!fin)
		return;

	HuffmanNode *cur = root;
	char buffer = 0;
	fin.read(&buffer, 1);
	int bitCount = 8;
	bool bit = 0;
	int result = 0;
	int checkEOF = 0;
	while (true) {
		result = 0;
		bit = 0;
		checkEOF = 0;

		if (bitCount == 0) {
			fin.read(&buffer, 1);
			if (!fin)
				return;
			bitCount = 8;
		}

		if (cur->isNYT == false) {
			bitCount--;
			bit = (buffer >> bitCount) & 1;
		}
		
		if (cur->value != -1) {
			fout << (char)cur->value;
			UpdateTreeModel(cur->value);
			cur = root;
			std::cout << "1";
		}
		else if (cur->isNYT) {
			result = (buffer << (8 - bitCount)) & 0xFF; 
			fin.read(&buffer, 1);	
			if (!fin)
				return;

			result |= (buffer >> bitCount);

			checkEOF = (int)((result << 1) | ((buffer >> (bitCount - 1)) & 1));
			if (checkEOF == AdaptiveHuffmanCoding::PSEUDO_EOF)
				return;

			fout << (char)result;
			UpdateTreeModel(result);
			cur = root;
			std::cout << "2";
		}
		else if(bit)
			cur = cur->right;
		else 
			cur = cur->left;
	}

	fin.close();
	fout.close();
}


AdaptiveHuffmanCoding::~AdaptiveHuffmanCoding() {
	DeleteHuffmanTree(root);
}

AdaptiveHuffmanCoding::HuffmanNode* AdaptiveHuffmanCoding::GetSymbolNode(int symbol, HuffmanNode *crr) const {
	if (crr == nullptr || crr->value == symbol) 
		return crr;

	HuffmanNode *leftResult = GetSymbolNode(symbol, crr->left);
	
	return leftResult == nullptr ? GetSymbolNode(symbol, crr->right) : leftResult;
}

void AdaptiveHuffmanCoding::FindBlockLeader(HuffmanNode *crr, HuffmanNode *&crrMax) const {
	if (crr == nullptr || crrMax == root) {
		return;
	}

	if (crr->weight == crrMax->weight && crr != crrMax->parent  && crr->order > crrMax->order) {
		crrMax = crr;
	}

	FindBlockLeader(crr->left, crrMax);
	FindBlockLeader(crr->right, crrMax);
}

void AdaptiveHuffmanCoding::SwapNodes(HuffmanNode *const node1, HuffmanNode *const node2) {
    if (node1->parent == nullptr || node2->parent == nullptr) return;
	if (node1->parent == node2 || node2->parent == node1) return;

    int node1Order = node1->order;
    node1->order = node2->order;
    node2->order = node1Order;

    bool node1IsLeftChild = false;
    if (node1->parent->left == node1) {
        node1IsLeftChild = true;
    }
    bool node2IsLeftChild = false;
    if (node2->parent->left == node2) {
        node2IsLeftChild = true;
    }

    if (node1IsLeftChild) {
        node1->parent->left = node2;
    } else {
        node1->parent->right = node2;
    }
    if (node2IsLeftChild) {
        node2->parent->left = node1;
    } else {
        node2->parent->right = node1;
    }

    HuffmanNode *node1Parent = node1->parent;
    node1->parent = node2->parent;
    node2->parent = node1Parent;
}

void AdaptiveHuffmanCoding::GetPathToSymbol(HuffmanNode *crr, HuffmanNode *result, int& code, int& len, int curCode, int curLen) const {
	if (crr == result) {
		code = curCode;
		len = curLen;
		return;
	}


	if (crr == nullptr)
		return;
	
	GetPathToSymbol(crr->left, result, code, len, curCode << 1, curLen + 1);
	GetPathToSymbol(crr->right, result, code, len, (curCode << 1) | 1, curLen + 1);
}

void AdaptiveHuffmanCoding::SlideAndIncrement(HuffmanNode *node) {
	if (node == nullptr)
		return;

	HuffmanNode *blockLeader = node;
	FindBlockLeader(root, blockLeader);
	if (blockLeader != node)
		SwapNodes(blockLeader, node);

	++node->weight;
	SlideAndIncrement(node->parent);
}

void AdaptiveHuffmanCoding::DeleteHuffmanTree(HuffmanNode *crrNode) {
	if (crrNode == nullptr) return;

	DeleteHuffmanTree(crrNode->left);
	DeleteHuffmanTree(crrNode->right);

	delete crrNode;
}