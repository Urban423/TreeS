#include "Window.h"
#include <stdio.h>
#include <math.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY: {
		PostQuitMessage(0);
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		break;
	}
	case WM_SYSKEYDOWN: {
		return 0;
	}
	case WM_MOUSEWHEEL: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->scale(HIWORD(wParam));
		break;
	}
	case WM_SIZING: { }
	case WM_SIZE: {
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->updateImage();
		break;
	}
	case WM_COMMAND:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		switch (LOWORD(wParam))
		{
			case(CLEAR_BUTTON): {
				window->clear();
				break;
			}
			case(REMOVE_BUTTON): {
				char* element = window->getTextField();
				if (element == 0) { break; }
				window->removeElement(element);
				break;
			}
			case(ADD_BUTTON): {
				char* element = window->getTextField();
				if (element == 0) { break; }
				window->addElement(element);
				break;
			}
			default: break;
		}
		break;
	}
	default: {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window() { }

Window::~Window() { DestroyWindow(_hwnd); }

void Window::init(const char* windowName, int width, int height, int x)
{
	GetCursorPos(&oldPos);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.lpszClassName = windowName;
	wc.lpszMenuName = "";
	wc.lpfnWndProc = WndProc;
	wc.style = 0;

	if (!RegisterClassEx(&wc)) {
		onDestroy();
	}

	RECT rc = { x, 0, width, height };
	AdjustWindowRect(&rc, WS_SYSMENU, false);

	_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		windowName, windowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
		rc.left, rc.top,
		rc.right, rc.bottom, NULL, NULL, NULL, NULL);

	SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)this);
	if (!_hwnd) {
		onDestroy();
	}

	GetClientRect(_hwnd, &rc);
	this->width = rc.right - rc.left - 160;
	this->height = rc.bottom - rc.top;
	left = rc.left;
	top = rc.top;


	brush = CreateSolidBrush(RGB(122, 122, 122));
	hDC = GetDC(_hwnd);
	bmp = CreateCompatibleBitmap(hDC, 2222, 2222);
	winCenterX = this->width / 2;
	winCenterY = this->height / 2;
	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);


	CreateWindow("static", "",
		WS_CHILD | WS_VISIBLE | ES_CENTER,
		0, 0, 160, 1200,
		_hwnd, NULL, NULL, NULL);

	CreateWindow("static", "Tree",
		WS_CHILD | WS_VISIBLE | ES_CENTER,
		20, 20, 120, 25,
		_hwnd, NULL, NULL, NULL);

	text_field = CreateWindow("edit", "", 
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 40, 120, 25, 
		_hwnd, NULL, NULL, NULL);
	
	CreateWindow("button", "Add",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
		20, 62, 60, 30,
		_hwnd, (HMENU)ADD_BUTTON, NULL, NULL);

	CreateWindow("button", "Remove",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
		80, 62, 60, 30,
		_hwnd, (HMENU)REMOVE_BUTTON, NULL, NULL);

	CreateWindow("button", "Clear",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
		20, 100, 120, 30,
		_hwnd, (HMENU)CLEAR_BUTTON, NULL, NULL);

	scale(0);
}

void Window::broadcast()
{
	RECT rc;
	GetWindowRect(_hwnd, &rc);
	oldPos = pos;
	old_state = new_state;
	GetCursorPos(&pos);
	new_state = GetKeyState(VK_LBUTTON) & 0x80;
	if (new_state && old_state == false)
	{
		if (pos.x > rc.left && pos.x < rc.right && pos.y > top && pos.y < rc.bottom)
		{
			move = true;
		}
	}
	if (new_state == false && old_state)
	{
		move = false;
	}
	if (move)
	{
		centerX += (pos.x - oldPos.x);
		centerY += (pos.y - oldPos.y);
		updateImage();
	}


	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::updateImage()
{
	RECT win_Size = getInnerSize();
	memDC = CreateCompatibleDC(hDC);
	HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);
	SelectObject(memDC, font);


	Rectangle(memDC, 0, -1, win_Size.right, win_Size.bottom);
	int rad = radius * size;
	for (int i = 0; i < nodesArray.size(); i++)
	{
		int x = winCenterX + (centerX + nodesArray[i].x) * size;
		int y = winCenterY + (centerY + nodesArray[i].y) * size;
		RECT text = { x - rad, y - rad, x + rad, y + rad };

		Ellipse(memDC, x - rad, y - rad, x + rad, y + rad);
		DrawText(memDC, &nodesArray[i].element, 1, &text, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (nodesArray[i].parentIndex == -1) continue;

		MoveToEx(memDC, winCenterX + (centerX + nodesArray[i].x2) * size, winCenterY + (centerY + nodesArray[i].y2) * size, NULL);
		LineTo(  memDC, winCenterX + (centerX + nodesArray[i].x1) * size, winCenterY + (centerY + nodesArray[i].y1) * size);
		LineTo(  memDC, winCenterX + (centerX + nodesArray[i].arrow_x1) * size, winCenterY + (centerY + nodesArray[i].arrow_y1) * size); 
		MoveToEx(memDC, winCenterX + (centerX + nodesArray[i].x1) * size, winCenterY + (centerY + nodesArray[i].y1) * size, NULL);
		LineTo(memDC, winCenterX + (centerX + nodesArray[i].arrow_x2) * size, winCenterY + (centerY + nodesArray[i].arrow_y2) * size);

	}


	BitBlt(hDC, 0, 0, width, height, memDC, -160, 0, SRCCOPY);
	SelectObject(memDC, oldBmp);
	DeleteDC(memDC);
}

void Window::scale(short delta){
	wheel += delta / 120;
	if (wheel < 1) wheel = 1;
	if (wheel > 300) wheel = 300;
	size = sqrt(wheel * 40) / 20.0f;

	DeleteObject(font);
	font = CreateFont(size * 40, 0, 0, 0, 300, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

	updateImage();
}

char* Window::getTextField() { GetWindowText(text_field, element, 100); return element; }

void Window::setTextField(int size) { SetWindowText(text_field, element); }

void Window::clear() { SetWindowText(text_field, ""); nodesArray.clear(); updateImage(); }

void Window::removeElement(char* str) { updateImage(); }

void Window::addElement(char* str) { updateImage(); }

void Window::calculateArrows() { for (int i = 0; i < nodesArray.size(); i++) { calculateArrow(nodesArray[i]); } }

RECT  Window::deltaCursorPos(){return { pos.x - oldPos.x, pos.y - oldPos.y };}

void Window::onDestroy(){}

RECT Window::getInnerSize()
{
	RECT rc;
	GetClientRect(_hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	left = rc.left;
	top = rc.top;
	return rc;
}

void Window::calculateArrow(miniNodes& a)
{
	if (a.parentIndex == -1) { return; }
	float rad = radius;
	miniNodes b = nodesArray[a.parentIndex];
	float x1 = a.x;
	float y1 = a.y;
	float x2 = b.x;
	float y2 = b.y;

	float dx = x2 - x1;
	float dy = y2 - y1;
	float length = sqrt(dx * dx + dy * dy);
	dx *= rad / length;
	dy *= rad / length;

	a.x1 = a.x + dx;
	a.y1 = a.y + dy;
	a.x2 = b.x - dx;
	a.y2 = b.y - dy;

	dx = (x2 - x1) * (rad * 0.2f) / length;
	dy = (y2 - y1) * (rad * 0.2f) / length;
	a.arrow_x1 = a.x1 + dx * cos(0.4f) - dy * sin(0.4f);
	a.arrow_y1 = a.y1 + dx * sin(0.4f) + dy * cos(0.4f);
	a.arrow_x2 = a.x1 + dx * cos(-0.4f) - dy * sin(-0.4f);
	a.arrow_y2 = a.y1 + dx * sin(-0.4f) + dy * cos(-0.4f);
}