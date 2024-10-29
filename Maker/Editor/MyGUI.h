#pragma once

#include "MyWindow.h"
#include <list>
#include <string>
using namespace std;

class MyGUI : public IEventProcessor
{
public:
	MyGUI(SDL_Window* window, void* context);
	MyGUI(MyGUI&&) noexcept = delete;
	MyGUI(const MyGUI&) = delete;
	MyGUI& operator=(const MyGUI&) = delete;
	~MyGUI();
	void render();
	list<string> _objects;
	void processEvent(const SDL_Event& event) override;

};
