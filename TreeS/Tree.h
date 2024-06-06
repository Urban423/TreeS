#pragma once
#include <vector>

struct Node
{
	unsigned char key;
	struct Node* left;
	struct Node* right;
};

struct miniNodes
{
	int x;
	int y;
	char element;
	int parentIndex;

	int x1; int y1;
	int x2; int y2;
	int arrow_x1; int arrow_y1;
	int arrow_x2; int arrow_y2;
};

class Tree
{
public:
	virtual  Node* insert(char* str) = 0;
	virtual  void removeNode(char* str) = 0;
	void clear();
	void calculateMiniNodesPos(std::vector<miniNodes>& nodesArray);
protected:
	Node* head;
	unsigned int size;
};