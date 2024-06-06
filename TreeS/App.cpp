#include "App.h"

App::App(Tree& trees, char* str, int number_of_elements, int xOffset) : tree(trees) {
	_running = true;
	offset = xOffset;
	for (int i = 0; i < number_of_elements; i++) {
		tree.insert(str);
	}
}

App::~App() { }

void App::onCreate()
{
	Window::init("Window", 640, 480, offset);
	tree.calculateMiniNodesPos(Window::nodesArray);
	Window::calculateArrows();
	Window::updateImage();
}

void App::onUpdate() { Window::broadcast(); }

bool App::isRunning() { return _running; }

void App::onDestroy() { _running = false; }

void App::clear()
{
	tree.clear();
	//DO SOMETHING
	Window::clear();
}

void App::removeElement(char* str)
{
	tree.removeNode(str);
	tree.calculateMiniNodesPos(Window::nodesArray);
	//DO SOMETHING
	Window::calculateArrows();
	Window::updateImage();
}

void App::addElement(char* str)
{
	tree.insert(str);
	tree.calculateMiniNodesPos(Window::nodesArray);
	//DO SOMETHING
	Window::setTextField(1);
	Window::calculateArrows();
	Window::updateImage();
}