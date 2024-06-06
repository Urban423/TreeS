#include "SybolicTree.h"

#define OK 0
#define CANT_FIND -1
#define ERROR_NUMBER_CORRUPTED 1

unsigned char getOperatorPriority(char character)
{
	switch (character)
	{
	case('-'):
	{

	}
	case('+'):
	{
		return 0;
	}
	case('*'):
	{

	}
	case('/'):
	{
		return 1;
	}
	case('^'):
	{
		return 2;
	}
	}
	return 255;
}

int findNextParenthesis(char* str, int start)
{
	int depth = 0;
	while (1) {
		if (str[start] == '(') { depth++; }
		else if (str[start] == ')') { depth--; if (depth == 0) { return start; } }
		start++;
	}
	return CANT_FIND;
}

bool checkParenthesis(char* str, int start, int end)
{
	if (str[start] != '(')
	{
		return false;
	}
	int i = findNextParenthesis(str, start);
	if (i == end)
	{
		return true;
	}
	return false;
}


Node* SymbolicTree::insert(char* str)
{
	int i = 0;
	for (; i < 100; i++) {
		if (str[i] == 0)
		{
			i--;
			break;
		}
	}
	clear();
	head = new Node();
	size = 1;

	parseString(head, str, 0, i);
	return nullptr;
}

void SymbolicTree::removeNode(char* str){}

void SymbolicTree::parseString(Node* node, char* str, int start, int end)
{
	if (str[start] == '+')
	{
		start++;
	}

	//loop to skip this situation: '((((a + b))))' -> 'a + b'
	while (checkParenthesis(str, start, end)) {
		start++;
		end--;
	}


	unsigned char min_priority = 0xFF;
	int min_priority_index = 0x7F'FF'FF'FF;

	//find operator
	for (int i = start; i <= end; i++)
	{
		//skip some arithmetic '()'
		if (str[i] == '(') {
			i = findNextParenthesis(str, i);
		}

		unsigned char priority = getOperatorPriority(str[i]);
		if (priority <= min_priority) {
			min_priority = priority;
			if (i < min_priority_index || end - i < end - min_priority_index) {
				min_priority_index = i;
			}
		}
	}



	//if operator was't found get value and parametr
	if (min_priority == 255)
	{
		node->left = nullptr;
		node->right = nullptr;
		node->key = str[start];
		return;
	}

	//if operator was found, create left and right child, then calculate them both
	node->key = str[min_priority_index];
	node->left = new Node();
	node->right = new Node();
	size += 2;


	parseString(node->right, str, start, min_priority_index - 1);
	parseString(node->left, str, min_priority_index + 1, end);
}