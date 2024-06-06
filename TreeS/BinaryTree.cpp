#include "BinaryTree.h"
#include <stdio.h>
#include <string>
using namespace std;

BinaryTree::BinaryTree() { head = nullptr; size = 0; }

BinaryTree::~BinaryTree(){}

Node* BinaryTree::insert(char* str)
{
	if (str[0] == 0) { return nullptr; }
	int si = 0;
	for (; si < 100; si++)
	{
		if (str[si] == 0) { break; }
	}
	char el = str[0];
	for (int i = 0; i < si; i++) {
		str[i] = str[i + 1];
	}

	size++;
	Node* new_node  = new Node();
	new_node->key   = el;
	new_node->left  = nullptr;
	new_node->right = nullptr;
	Node* temp      = head;
	Node* pred      = nullptr;
	if(head == nullptr)
	{
		head = new_node;
		return new_node;
	}
	
	while(temp != nullptr)
	{
		pred = temp;
		if(temp->key > el) {
			temp = temp->right;
		}
		else {
			temp = temp->left;
		} 
	}
	
	if(pred->key > el) {
		pred->right = new_node;
	}
	else {
		pred->left  = new_node;
	}
	
	return new_node;
}

Node* getMinimumKey(Node* p, Node*& parent)
{ 
	while (p->left != nullptr) 
	{
		parent = p;
        p = p->left;
    }
    return p;
}

void  BinaryTree::removeNodeR(Node* p, Node* parent, Node* node)
{
	if(node == p)
	{
		int countChildren = (p->left != nullptr) + (p->right != nullptr);
		Node* childPointer = (Node*)((long long unsigned int)p->left + (long long unsigned int)p->right);
		switch(countChildren)
		{
			case(0):
			{
				
			}
			case(1):
			{
				if(parent != nullptr)
				{
					if(p == parent->right)
					{
						parent->right = childPointer;
					}
					else{
						parent->left  = childPointer;
					}
				}
				break;
			}
			case(2):
			{
				Node* sParent = p;
				Node* s = getMinimumKey(p->right, sParent);
				p->key = s->key;
				
				removeNodeR(s, sParent, s);
				
				return;
			}
		}
		delete p;
		return;
	}
	
	if(node->key < p->key)
	{
		removeNodeR(p->right, p, node);
	}
	else
	{
		removeNodeR(p->left,  p, node);
	}
}

void BinaryTree::removeNode(char* str)
{
	if (size == 0) { return; }
	if (str[0] == 0) { return; }
	size--;
	Node* node = find(str[0]);
	return removeNodeR(head, nullptr, node);
}

Node* BinaryTree::findByKey(Node* p, unsigned char key)
{
	if(p == nullptr) {
		return nullptr;
	}
	
	
	if(key == p->key)
	{
		return p;
	}
	if(p->key > key)
	{
		return findByKey(p->right, key);
	}
	else
	{
		return findByKey(p->left,  key);
	}
}

Node* BinaryTree::find(unsigned char element)
{
	if(size == 0) { return nullptr; }
	return findByKey(head, element);
}

Node* BinaryTree::getRoot() { return head; }

unsigned int heightOfUnderTree(Node* p, unsigned int size)
{
	if(p == nullptr) { return size; }
	size++;
	unsigned int height = heightOfUnderTree(p->left, size);
	unsigned int height1 = heightOfUnderTree(p->right, size);
	if(height1 > height)
	{
		return height1;
	}
	return height;
}

unsigned int BinaryTree::getHeight()
{
	if(head == nullptr) { return 0; }
	return heightOfUnderTree(head, 0);
}

unsigned int BinaryTree::getSize() { return size; }