#pragma once
#include "Window.h"

class App: Window
{
public:
	App(Tree& trees, char* str, int number_of_elements, int xOffset);
	~App();
	
	void onCreate();
	void onUpdate();
	bool isRunning();

	void removeElement(char* str);
	void addElement(char* str);
	void onDestroy();
	void clear();
private:
	Tree& tree;
	int offset;
	bool _running = false;
};