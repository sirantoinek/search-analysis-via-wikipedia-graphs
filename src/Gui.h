// referenced https://youtu.be/U1BnzWX194Q?si=tMxZHqrBkEy5TYx1 to set up nearly all of ImGui (repo for the guide: https://github.com/codetechandtutorials/imGUIexample)
// (vcpkg.json is entirely cited from this video)
// as well as https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp (imgui example program)

#pragma once

#include <imgui.h>
#include "imgui_stdlib.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>

class Gui
{
private:
	// add private helper functions here to be used in the public functions.

public:
	void init(GLFWwindow* window, const char* glsl_version);
	void update(GLFWwindow* window, Algorithms& wikiDatabase);
	void render();
	void shutdown();

};

// Class Function Definitions ---------------------

void Gui::init(GLFWwindow* window, const char* glsl_version) // mostly retrieved from https://youtu.be/U1BnzWX194Q?si=tMxZHqrBkEy5TYx1
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}

void Gui::update(GLFWwindow* window, Algorithms& wikiDatabase)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags window_flags = 0; //setting flags for Begin() referenced https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	int windowWidth, windowHeight;
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight); // referenced https://www.glfw.org/docs/3.3/window_guide.html (Framebuffer size)
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	// Options window:

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // locking options windows to the top left
	ImGui::SetNextWindowSize(ImVec2(main_viewport->WorkPos.x, float(windowHeight)), ImGuiCond_Always); // locking options window width to a set value and height to scale as the page is resized

	ImGui::Begin("Options:", NULL, window_flags);  // Create a window

	ImGui::Text("Wiki Article Selection:"); // Display some text (you can use a format strings too)// Edit bools storing our window open/close state
	ImGui::SameLine();
	ImGui::TextDisabled("(?)"); //setting up a tooltip retrieved/ referenced from https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Titles are case sensitive!\n\nAlso be aware that if the page shows \"Not Responding\",\nit is likely still processing in the background!");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	static char bufFrom[200] = ""; // set buffer to 200 as all titles are < 200 characters
	static char bufTo[200] = "";
	ImGui::InputTextWithHint("Starting Wiki Article", "enter title here",     bufFrom, 200); //setting up text input referenced from https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	ImGui::InputTextWithHint("Target Wiki Article", "enter title here",     bufTo, 200);

	ImGui::Text(" ");

	ImGui::Text("Select Searching Algorithm:");
	static int e = 0; //radio button setup retrieved/ referenced from https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	ImGui::RadioButton("Breadth-first search", &e, 0);
	ImGui::RadioButton("Dijkstra's Algorithm", &e, 1);
	ImGui::RadioButton("Bellman-Ford Algorithm", &e, 2);

	ImGui::Text(" ");

	static bool pressed = false, invalid = false;
	static pair<pair<int, double>, vector<string>> results;
	if(ImGui::Button("Find Shortest Connecting Path!")) //button setup referenced https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	{
		pressed = true; // used later in displaying results
		invalid = false; // used later in displaying results
		std::string from(bufFrom, strlen(bufFrom)); // converts char array to string to be used in searches
		std::string to(bufTo, strlen(bufTo));

		// validates the from and to article titles
		if (wikiDatabase.getID.find(from) == wikiDatabase.getID.end() || wikiDatabase.getID.find(to) == wikiDatabase.getID.end())
		{
			pressed = false;
			invalid = true;
		}

		if(!invalid) // runs a search based on the position of the radio buttons
		{
			if(e == 0) results = wikiDatabase.breadthFirstSearch(from, to);
			else if(e == 1) results = wikiDatabase.dijkstraSearch(from, to);
			else if(e == 2) results = wikiDatabase.bellmanFordSearch(from, to);
		}
	}

	ImGui::Text(" ");

	ImGui::Text("Results:");
	ImGui::Text("Shortest Path Length:");
	if(pressed & !invalid)
	{
		ImGui::SameLine();
		if(results.second.empty()) ImGui::Text("No connection found");
		else ImGui::Text("%d", int(results.second.size() - 1));
	}
	ImGui::Text("Number of Nodes Inspected:");
	if(pressed & !invalid)
	{
		ImGui::SameLine();
		ImGui::Text("%d", results.first.first);
	}
	ImGui::Text("Runtime:");
	if(pressed & !invalid)
	{
		ImGui::SameLine();
		ImGui::Text("%f", results.first.second);
		ImGui::SameLine();
		ImGui::Text("seconds");
	}
	if(invalid) // displays a message if an article title is invalid
	{
		ImGui::Text(" ");
		ImGui::Text("At least one of the given titles does not exist.");
		ImGui::Text("Please try again.");
	}

	ImVec2 optionsWindowSize = ImGui::GetWindowSize(); // referenced imgui demo code https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	ImGui::End();

	// Page Links window:

	ImGui::SetNextWindowPos(ImVec2(optionsWindowSize.x, 0), ImGuiCond_Always); // locking page links window to be directly to the right of options window
	ImGui::SetNextWindowSize(ImVec2(float(windowWidth) - optionsWindowSize.x, float(windowHeight)), ImGuiCond_Always); // setting up page links window to scale in size as the page is resized

	ImGui::Begin("Page Links:", NULL, window_flags); // Create a window

	ImGui::Text("Display page links here:");

	ImGui::End();

}

void Gui::render() // mostly retrieved from https://youtu.be/U1BnzWX194Q?si=tMxZHqrBkEy5TYx1
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// glfwSwapBuffers() called in main
}

void Gui::shutdown() // mostly retrieved from https://youtu.be/U1BnzWX194Q?si=tMxZHqrBkEy5TYx1
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate(); // referenced https://www.glfw.org/docs/3.3/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901
}