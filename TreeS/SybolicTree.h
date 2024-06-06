#pragma once
#include "Tree.h"


class SymbolicTree : public Tree
{
public:
	Node* insert(char* str);
	void removeNode(char* str);


	void parseString(Node* node, char* str, int start, int end);
};