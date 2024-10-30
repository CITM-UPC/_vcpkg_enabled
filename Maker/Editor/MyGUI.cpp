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



FileManager fileManager;
GameObject* selectedGameObject = nullptr; // Define selectedGameObject

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
					GameObject go;
                    fileManager.LoadFile(filePath, go);
					scene.emplaceChild(go);
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
        for (auto& child : scene.getChildren()) {
            static char newName[128] = "";
            static bool renaming = false;
            static GameObject* renamingObject = nullptr;

            if (renaming && renamingObject == &child) {
                ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText("##rename", newName, IM_ARRAYSIZE(newName), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    child.name = newName;
                    renaming = false;
                }
                if (ImGui::IsItemDeactivated() || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                    renaming = false;
                }
            }
            else {
                if (ImGui::Selectable(child.name.c_str(), selectedGameObject == &child)) {
                    selectedGameObject = &child;
                }
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    renaming = true;
                    renamingObject = &child;
                    strcpy_s(newName, child.name.c_str());
                }
            }
        }
    }
    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(500, 700), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(780, 20), ImGuiCond_Always);
    if (ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        if (selectedGameObject) {
            ImGui::Text("Selected GameObject: %s", selectedGameObject->name.c_str());

            // Display position
            ImGui::Text("Position");
            float position[3] = { static_cast<float>(selectedGameObject->_transform.pos().x),
                      static_cast<float>(selectedGameObject->_transform.pos().y),
                      static_cast<float>(selectedGameObject->_transform.pos().z) };

            if (ImGui::InputFloat3("##position", position)) {
                selectedGameObject->_transform.pos() = glm::vec3(position[0], position[1], position[2]);
            }


            
        }
        else {
            ImGui::Text("No GameObject selected.");
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