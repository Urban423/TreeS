#pragma once
#include "Tree.h"

class BinaryTree: public Tree
{
public:
	BinaryTree();
	~BinaryTree();
	
	Node* insert(char* str);
	void removeNode(char* str);
	Node* find(unsigned char element);
	Node* getRoot();
	unsigned int getHeight();
	unsigned int getSize();
private:
	Node* findByKey(Node* p, unsigned char key);
	void  removeNodeR(Node* p, Node* parent, Node* node);
};