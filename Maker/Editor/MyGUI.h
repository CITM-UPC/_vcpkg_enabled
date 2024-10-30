#pragma once

#include "MyWindow.h"
#include "Engine/Scene.h"
#include <list>
#include <string>
#include <vector> // Include the vector header
#include <imgui.h>

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
    void processEvent(const SDL_Event& event) override;
private:
   


};
