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
#include <unordered_set>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct BoundingBox {
    ImVec2 min;
    ImVec2 max; // the max and min of a box represent the furthest points from the center of a node that will still result in a collision with another node

    bool intersects(BoundingBox& other) {
        return !(max.x < other.min.x || min.x > other.max.x || max.y < other.min.y || min.y > other.max.y);
    }
};

// op overloading referenced from https://cplusplus.com/doc/tutorial/templates/
bool operator==(ImVec2& lhs, ImVec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

class Gui
{
private:
    unsigned int randomSeed;
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

    randomSeed = 1234; // generate random seed here instead of in update so that we don't constantly find different random paths
    srand(randomSeed);
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

    static bool random = false;
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::SetCursorPos(ImVec2(10, windowSize.y - 50));
    ImGui::Text("Node Layout (experimental)");
    ImGui::SetCursorPos(ImVec2(10, windowSize.y - 30));
    if (ImGui::Button("Randomize Layout if Available")) {
        random = !random;
    }

	ImVec2 optionsWindowSize = ImGui::GetWindowSize(); // referenced imgui demo code https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	ImGui::End();

	// Page Links window:

	ImGui::SetNextWindowPos(ImVec2(optionsWindowSize.x, 0), ImGuiCond_Always); // locking page links window to be directly to the right of options window
	ImGui::SetNextWindowSize(ImVec2(float(windowWidth) - optionsWindowSize.x, float(windowHeight)), ImGuiCond_Always); // setting up page links window to scale in size as the page is resized

    ImGui::Begin("Page Links:", NULL, window_flags);  // Create a window

    if (invalid) {
        ImGui::Text("Invalid title(s) entered");
        ImGui::End();
        return;
    }

    if(!random){ // if random flag is not set, then display nodes in a snaking pattern
        float baseOvalHeight = 50, verticalArrowLength = 70, leftBoundary = 400.0f;
        float initialX = leftBoundary, initialY = 60.0f, arrowPadding = 40.0f;

        float firstOvalWidth = 100;
        if (results.second.size() > 0 && results.second[0].length() > 10) {
            firstOvalWidth += (results.second[0].length() - 10) * 10;
        }
        initialX += firstOvalWidth / 2;

        ImVec2 position(initialX, initialY);
        vector<BoundingBox> occupiedBounds;

        vector<ImVec2> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, 1} }; // right, down, left, down (up never used)
        int currentDirection = 0;

        vector<ImVec2> ovalCenters;
        vector<ImVec2> arrowStartPoints;
        vector<ImVec2> arrowEndPoints;
        vector<string> texts = results.second;

        ImGui::BeginGroup();
        for (int i = 0; i < results.second.size(); ++i){ // iterates through all nodes
            float ovalWidth = 100;
            if (results.second[i].length() > 10){
                ovalWidth += (results.second[i].length() - 10) * 10;
            }

            float nextOvalWidth = 100;
            if (i + 1 < results.second.size() && results.second[i + 1].length() > 10){
                nextOvalWidth += (results.second[i + 1].length() - 10) * 10;
            }

            ImVec2 nextPosition = position;
            if (directions[currentDirection].x != 0) { // calculates our next position based off current direction and next oval width
                nextPosition.x += directions[currentDirection].x * (ovalWidth / 2 + nextOvalWidth / 2 + arrowPadding);
            }
            else {
                nextPosition.y += directions[currentDirection].y * (baseOvalHeight + verticalArrowLength);
            }

            if ((currentDirection == 0 && nextPosition.x + nextOvalWidth / 2 > windowWidth) || (currentDirection == 2 && nextPosition.x - nextOvalWidth / 2 < leftBoundary)){
                if (currentDirection == 0) { // r - d
                    nextPosition.x = windowWidth - nextOvalWidth / 2 - 20;
                }
                else if (currentDirection == 2) { // l - d
                    nextPosition.x = leftBoundary + nextOvalWidth / 2;
                }

                currentDirection = (currentDirection + 1) % 4;
                nextPosition.y = position.y + baseOvalHeight + verticalArrowLength;
            }

            ovalCenters.push_back(position);
            position = nextPosition;

            if (directions[currentDirection].x == 0) {
                currentDirection = (currentDirection + 1) % 4;
            }

            if (i > 0){
                arrowStartPoints.push_back(ovalCenters[i - 1]);
                arrowEndPoints.push_back(ovalCenters[i]);
            }
        }

        for (size_t i = 0; i < arrowStartPoints.size(); i++){ // arrow drawing
            ImGui::GetWindowDrawList()->AddLine(arrowStartPoints[i], arrowEndPoints[i], IM_COL32(0, 100, 255, 255), 2.0f);
        }

        for (size_t i = 0; i < ovalCenters.size(); i++){ // drawing the ovals
            ImVec2 ovalCenter = ovalCenters[i];
            float ovalWidth = 100;
            if (texts[i].length() > 10){
                ovalWidth += (texts[i].length() - 10) * 10; // increases the size of the ovals if the string is too big
            }
            float ovalRadiusX = ovalWidth / 2;
            float ovalRadiusY = baseOvalHeight / 2;
            const int num_segments = 50;
            ImVector<ImVec2> ellipse_points;
            for (int j = 0; j < num_segments; j++){
                float theta = 2.0f * M_PI * float(j) / float(num_segments); // get angle
                float x = ovalRadiusX * cosf(theta); // calculate x component
                float y = ovalRadiusY * sinf(theta); // calculate y component
                ellipse_points.push_back(ImVec2(ovalCenter.x + x, ovalCenter.y + y)); // output vertex
            }
            ImGui::GetWindowDrawList()->AddConvexPolyFilled(ellipse_points.Data, ellipse_points.Size, IM_COL32(0, 100, 255, 255));
        }

        for (size_t i = 0; i < ovalCenters.size(); i++){ // draws all text last so that it isn't drawn on top of
            ImVec2 textPos = ImVec2(ovalCenters[i].x - ImGui::CalcTextSize(texts[i].c_str()).x / 2,
                                    ovalCenters[i].y - ImGui::CalcTextSize(texts[i].c_str()).y / 2);
            ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), texts[i].c_str());
        }
    }

    else{ // if random is true then display the nodes in a random fashion
        const float baseOvalHeight = 50; // all ovals have base dimensions that will be increased if the strings they hold are too big
        const float arrowLength = 30;
        windowWidth = ImGui::GetWindowSize().x;
        windowHeight = ImGui::GetWindowSize().y;
        const float leftBoundary = 400.0f;

        ImVec2 position(leftBoundary, 40);
        vector<BoundingBox> occupiedBounds; // stores a vector of bounds that are occupied so that nodes don't intersect with each other and must select a different path

        srand(randomSeed); // this sets random seed to the same value every time update is called since init() was used

        vector<ImVec2> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }; // node movement directions
        int previousDirection = -1;

        vector<ImVec2> ovalCenters; // vectors of ImVec2 are used to store the coordinates of each respective object
        vector<ImVec2> arrowStartPoints;
        vector<ImVec2> arrowEndPoints;
        vector<string> texts;

        ImGui::BeginGroup();
        for (int i = 0; i < results.second.size(); i++){ // iterates over all nodes

            float ovalWidth = 100;
            if (results.second[i].length() > 10){
                ovalWidth += (results.second[i].length() - 10) * 10; // this is where the oval length is increased based on string size
            }

            ImVec2 ovalCenter = ImVec2(position.x + ovalWidth / 2, position.y + baseOvalHeight / 2);
            ovalCenters.push_back(ovalCenter);
            texts.push_back(results.second[i]);
            BoundingBox currentBox = { {position.x, position.y}, {position.x + ovalWidth, position.y + baseOvalHeight} };
            occupiedBounds.push_back(currentBox);

            if (i < results.second.size() - 1){
                ImVec2 succPos;
                BoundingBox succBox;
                int direction;
                int count = 0;
                while (true) { // this while loop continues to iterate until a valid path is found for the following node OR count reaches 100 indicating that no valid path is possible
                    direction = rand() % 4;
                    float moveX = ovalWidth + arrowLength;
                    float moveY = baseOvalHeight + arrowLength;
                    succPos = ImVec2(position.x + directions[direction].x * moveX, position.y + directions[direction].y * moveY);

                    float nextOvalWidth = 100;
                    if (results.second[i + 1].length() > 10){
                        nextOvalWidth += (results.second[i + 1].length() - 10) * 10;
                    }

                    succBox = { {succPos.x, succPos.y}, {succPos.x + nextOvalWidth, succPos.y + baseOvalHeight} }; // changes the bounding box parameters of succBox after finding new path

                    bool intersectsAnyBox = any_of(occupiedBounds.begin(), occupiedBounds.end(), [&](BoundingBox& box) {
                        return succBox.intersects(box);
                    });

                    bool isOppositeDirection = direction == (previousDirection + 2) % 4;

                    bool exceedsRightBoundary = succBox.max.x > windowWidth;
                    bool exceedsLeftBoundary = succBox.min.x < leftBoundary;
                    bool exceedsBottomBoundary = succBox.max.y > windowHeight;
                    bool exceedsTopBoundary = succBox.min.y < 0;
                    count++;

                    if(count > 100){ // count increments for every iteration of the while loop
                        count = 0;
                        random = false;
                        break;
                    }

                    if (!intersectsAnyBox && !isOppositeDirection && !exceedsRightBoundary && !exceedsLeftBoundary && !exceedsBottomBoundary && !exceedsTopBoundary) {
                        count = 0;
                        break; // this is the exit of the while loop. if there are no valid paths, for example if a node is surrounded by 4 other nodes then there will be no valid path and the program will run indefinitely
                    }
                }

                arrowStartPoints.push_back(ovalCenter);
                float nextOvalWidth = 100; // base dimension
                if (results.second[i + 1].length() > 10) nextOvalWidth += (results.second[i + 1].length() - 10) * 10;
                arrowEndPoints.push_back(ImVec2(succPos.x + nextOvalWidth / 2, succPos.y + baseOvalHeight / 2));

                position = succPos;
                occupiedBounds.push_back(succBox); // adds new bounding box to list of occupied bounds
                previousDirection = direction;
            }
        }

        for (size_t i = 0; i < arrowStartPoints.size(); i++){ // arrow drawing
            ImGui::GetWindowDrawList()->AddLine(arrowStartPoints[i], arrowEndPoints[i], IM_COL32(0, 100, 255, 255), 2.0f);
        }

        for (size_t i = 0; i < ovalCenters.size(); i++){ // drawing the ovals
            ImVec2 ovalCenter = ovalCenters[i];
            float ovalWidth = 100;
            if (texts[i].length() > 10) ovalWidth += (texts[i].length() - 10) * 10; // increases the size of the ovals if the string is too big

            float ovalRadiusX = ovalWidth / 2;
            float ovalRadiusY = baseOvalHeight / 2;
            const int num_segments = 50;
            ImVector<ImVec2> ellipse_points;
            for (int j = 0; j < num_segments; j++){
                float theta = 2.0f * M_PI * float(j) / float(num_segments); // get angle
                float x = ovalRadiusX * cosf(theta); // calculate x component
                float y = ovalRadiusY * sinf(theta); // calculate y component
                ellipse_points.push_back(ImVec2(ovalCenter.x + x, ovalCenter.y + y)); // output vertex
            }
            ImGui::GetWindowDrawList()->AddConvexPolyFilled(ellipse_points.Data, ellipse_points.Size, IM_COL32(0, 100, 255, 255));
        }

        for (size_t i = 0; i < ovalCenters.size(); i++){ // draws all text last so that it isn't drawn on top of
            ImVec2 textPos = ImVec2(ovalCenters[i].x - ImGui::CalcTextSize(texts[i].c_str()).x / 2,
                                    ovalCenters[i].y - ImGui::CalcTextSize(texts[i].c_str()).y / 2);
            ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), texts[i].c_str());
        }
    }

    ImGui::EndGroup();

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