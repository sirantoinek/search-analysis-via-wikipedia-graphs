#include "Algorithms.h"
#include "Gui.h"

int main()
{
	cout << "Deserializing binaries, please wait..." << endl;
	Algorithms wikiDatabase; // is an algorithms object so that it can access functions from algs and wikidata
	cout << "Done!" << endl;
	//wikiDatabase.algTesting(); // runs bfs test code

	// GUI STUFF ---------

	// window code retrieved from imgui example https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp AND https://github.com/codetechandtutorials/imGUIexample
	glfwInit();
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Search Analysis Via Wikipedia Graphs", nullptr, nullptr);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // initializing glad

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	Gui imGui;
	imGui.init(window, glsl_version);
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

		glClear(GL_COLOR_BUFFER_BIT);
		imGui.update(window, wikiDatabase);
		imGui.render();
		glfwSwapBuffers(window);
	}
	imGui.shutdown(window);
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
