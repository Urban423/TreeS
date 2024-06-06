#pragma once
#include "Tree.h"
#include <Windows.h>

#define CLEAR_BUTTON	1
#define ADD_BUTTON		2
#define REMOVE_BUTTON	3

class Window
{
public:
	Window();
	~Window();
	
	//size
	RECT getInnerSize();
	
	//graphic
	void calculateArrow(miniNodes& a);
	void calculateArrows();

	//main
	RECT deltaCursorPos();
	void init(const char* windowName, int width, int height, int x);
	void broadcast();
	void updateImage();
	void scale(short delta);
	char* getTextField();
	void setTextField(int size);

	virtual void clear();
	virtual void removeElement(char* str);
	virtual void addElement(char* str);
	virtual void onDestroy();
protected:
	float size;
	int centerX = 0;
	int centerY = -80;
	int radius = 30;
	char element[100];

	bool move = 0;
	short new_state;
	short old_state;
	HFONT font;
	int wheel = 30;
	POINT oldPos;
	POINT pos;
	std::vector<miniNodes> nodesArray;
	HBITMAP bmp;
	HBRUSH brush;
	HWND text_field;
	HWND _hwnd;
	HDC memDC;
	HDC hDC;
	
	int winCenterX; int winCenterY;
	int width; int height;
	int left;  int top;
};