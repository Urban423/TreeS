#include "App.h"
#include "SybolicTree.h"
#include "BinaryTree.h"

int main()
{
	const char* stringe = "dbacfeg";
	char* str = new char[sizeof(stringe)];
	for (int i = 0; i < sizeof(stringe); i++) {
		str[i] = stringe[i];
	}
	str[sizeof(stringe) - 1] = 0;
	App app[2] = { App(*(new BinaryTree()), str, sizeof(stringe) - 1, 0), App(*(new SymbolicTree()), (char*)"2*(x*y*i)", 1, 600)};
	app[0].onCreate();
	app[1].onCreate();
	while (app[0].isRunning())
	{
		app[0].onUpdate();
		app[1].onUpdate();
	}
	return 0;
}