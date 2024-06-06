#include "Tree.h"


void clearNode(Node* node)
{
	if (node == nullptr) { return; }
	clearNode(node->left);
	clearNode(node->right);
	delete node;
}


void Tree::clear()
{
	clearNode(head);
	head = nullptr;
	size = 0;
}


int calculateMiniNodePos(std::vector<miniNodes>& nodesArray, Node* node, int& leafsX, int& index, int parent_index, int scale, int depth)
{
	int a, b;
	int my_index = index++;
	if (node->left == nullptr && node->right != nullptr)
	{
		b = calculateMiniNodePos(nodesArray, node->right, leafsX, index, my_index, scale, depth + 1);
		nodesArray[my_index] = { b,  depth * scale, (char)node->key, parent_index };
		return b;
	}
	if (node->left != nullptr && node->right == nullptr)
	{
		a = calculateMiniNodePos(nodesArray, node->left, leafsX, index, my_index, scale, depth + 1);
		nodesArray[my_index] = { a,  depth * scale, (char)node->key, parent_index };
		return a;
	}
	else if (node->left != nullptr && node->right != nullptr)
	{
		a = calculateMiniNodePos(nodesArray, node->left, leafsX, index, my_index, scale, depth + 1);
		b = calculateMiniNodePos(nodesArray, node->right, leafsX, index, my_index, scale, depth + 1);
		nodesArray[my_index] = { (a + b) / 2,  depth * scale, (char)node->key, parent_index };
		return (a + b) / 2;
	}
	nodesArray[my_index] = { (leafsX--) * scale,  depth * scale, (char)node->key, parent_index };
	return (leafsX + 1) * scale;
}


void Tree::calculateMiniNodesPos(std::vector<miniNodes>& nodesArray)
{
	if (head == nullptr) { nodesArray.resize(0);  return; }
	nodesArray.resize(size);
	int index = 0;
	int leafsX = 0;
	calculateMiniNodePos(nodesArray, head, leafsX, index, -1, 70, 0);
}