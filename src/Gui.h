// referenced https://youtu.be/U1BnzWX194Q?si=tMxZHqrBkEy5TYx1 to set up ImGui
// as well as https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp (imgui example program)
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Gui
{
private:
	// add private helper functions here to be used in the public functions.

public:
	void init(GLFWwindow* window, const char* glsl_version);
	void update(GLFWwindow* window);
	void render();
	void shutdown(GLFWwindow* window);

};

// Class Function Definitions ---------------------

void Gui::init(GLFWwindow* window, const char* glsl_version)
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

void Gui::update(GLFWwindow* window)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	static float f = 0.0f;
	static int counter = 0;

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

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(main_viewport->WorkPos.x, float(windowHeight)), ImGuiCond_Always);

	ImGui::Begin("Options:", NULL , window_flags);  // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)// Edit bools storing our window open/close state
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImVec2 optionsWindowSize = ImGui::GetWindowSize(); // referenced imgui demo code https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp

	ImGui::End();

	// Page Links window:

	ImGui::SetNextWindowPos(ImVec2(optionsWindowSize.x, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(float(windowWidth) - optionsWindowSize.x, float(windowHeight)), ImGuiCond_Always);

	ImGui::Begin("Page Links:", NULL, window_flags);  // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Display page links here:");               // Display some text (you can use a format strings too)// Edit bools storing our window open/close state

	ImGui::End();

}

void Gui::render()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls glfwSwapBuffers() etc.)
}

void Gui::shutdown(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}