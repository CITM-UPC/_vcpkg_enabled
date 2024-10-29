#include "MyGUI.h"
#include "Engine/FileManager.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <tinyfiledialogs/tinyfiledialogs.h> 

MyGUI::MyGUI(SDL_Window* window, void* context) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init();
}

MyGUI::~MyGUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}




// MyGUI.cpp

void MyGUI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Import FBX")) {
                // Open file dialog to select an FBX file
                const char* filterPatterns[1] = { "*.fbx" };
                const char* filePath = tinyfd_openFileDialog(
                    "Select an FBX file",
                    "",
                    1,
                    filterPatterns,
                    NULL,
                    0
                );
                if (filePath) {
                    std::string file = filePath;
                    file = file.substr(file.find_last_of("\\") + 1);

                    // Remove .fbx extension
                    if (file.size() > 4 && file.substr(file.size() - 4) == ".fbx") {
                        file = file.substr(0, file.size() - 4);
                    }

                    // Ensure unique name
                    std::string uniqueFile = file;
                    int counter = 1;
                    while (std::find(_objects.begin(), _objects.end(), uniqueFile) != _objects.end()) {
                        uniqueFile = file + "(" + std::to_string(counter++) + ")";
                    }

                    _objects.push_back(uniqueFile);
                    // Handle the selected file path
                    // For example, you can load the FBX file here
                    // loadFBX(filePath);
                }
            }
            ImGui::EndMenu(); // Close the "File" menu
        }
        ImGui::EndMainMenuBar(); // Close the main menu bar
    }

    ImGui::SetNextWindowSize(ImVec2(300, 700), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
    if (ImGui::Begin("Gameobjects", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        for (int i = 0; i < _objects.size(); ++i) {
            if (ImGui::Selectable(_objects[i].c_str(), _selectedObjectIndex == i)) {
                _selectedObjectIndex = i;
                _isRenaming = false;
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                _isRenaming = true;
                strncpy_s(_renameBuffer, _objects[_selectedObjectIndex].c_str(), sizeof(_renameBuffer));
                _renameBuffer[sizeof(_renameBuffer) - 1] = '\0';
                ImGui::SetKeyboardFocusHere(); // Set focus to the input text field
            }
            // Capture the position of the selected item
            if (_selectedObjectIndex == i) {
                _selectedItemPos = ImGui::GetItemRectMin();
                _selectedItemSize = ImGui::GetItemRectSize();
            }
        }
        if (_selectedObjectIndex != -1 && _isRenaming) {
            // Calculate the position for the renaming input box
            ImVec2 renamePos = ImVec2(_selectedItemPos.x, _selectedItemPos.y + _selectedItemSize.y);

            // Set the position of the renaming input box
            ImGui::SetNextWindowPos(renamePos);

            ImGui::Begin("Rename", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SetKeyboardFocusHere(); // Ensure the text is selected
            ImGui::InputText("##Rename", _renameBuffer, sizeof(_renameBuffer));
            if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                _objects[_selectedObjectIndex] = _renameBuffer;
                _isRenaming = false;
                _selectedObjectIndex = -1; // Deselect the object after renaming
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                _isRenaming = false;
                _selectedObjectIndex = -1; // Deselect the object after canceling
            }
            ImGui::End();
        }
    }
    ImGui::End();
    //ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}











void MyGUI::processEvent(const SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}