
/*Standard Includes*/
#include<iostream>
#include <iterator>
#include <vector>
#include <array>
#include <cstdlib>
#include <utility>
/*Stats*/
#include <time.h>
#include <chrono>
#include <ctime> 

 
/*Point generation*/
#include"thinks/poisson_disk_sampling/poisson_disk_sampling.h"
/*Delaunay trianglulation*/
#include<delaunator-cpp/delaunator.hpp>


/*Gui Includes for openGL3*/
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

/*OpenGL Includes*/
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Main.h"



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor; // output a color to the fragment shader;\n"
"uniform float size;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(size * aPos.x, size * aPos.y, size * aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
//"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor,1);\n"
"}\n\0";


std::vector<double> construct_voronoi_edges(const delaunator::Delaunator& delaunay) {

	std::vector<double> line_edges;

	for (size_t e = 0; e < delaunay.triangles.size()/3; e++ ) {
		if (e < 2 * delaunay.triangles[delaunay.halfedges[e]]) {
			const auto p = delaunator::circumcenter(delaunay.coords[2 * delaunay.triangles[e]], delaunay.coords[2 * delaunay.triangles[e]+1],
				delaunay.coords[2 * delaunay.triangles[e+1]], delaunay.coords[2 * delaunay.triangles[e+1]+1], 2 * delaunay.coords[delaunay.triangles[e+2]], delaunay.coords[2 * delaunay.triangles[e+2]+1]);

			const auto q = delaunator::circumcenter(delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e]]],delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e]] + 1],  delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e + 1] ]],  delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e + 1] ] + 1],  delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e + 2] ] ], delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e + 2] ] + 1]);
			
			line_edges.push_back((double)e);
			line_edges.push_back(p.first);
			line_edges.push_back(p.second);
			line_edges.push_back(q.first);
			line_edges.push_back(q.second);

		}
	}
	unsigned int index = 0;
	std::cout << "Beep Boop, Printing....." << '\n';
	for (auto i : line_edges)
	{

		index++;
		if ((index % 5) != 0) {
			std::cout << i << ',';
		}
		else
		{

			std::cout << i << ',' << '\n';
		};

	}
	return line_edges;
}

//auto construct_voronoi_polygons() {
//
//	function edgesAroundPoint(delaunay, start) {
//		const result = [];
//		let incoming = start;
//		do {
//			result.push(incoming);
//			const outgoing = nextHalfedge(incoming);
//			incoming = delaunay.halfedges[outgoing];
//		} while (incoming != = -1 && incoming != = start);
//		return result;
//	}
//}
std::vector<std::array<float, 2>> generate_points()
{

	// Input parameters.
	constexpr float max_val = 0.900f;
	constexpr float kRadius = 0.088; //0.008 seems good
	constexpr auto kXMin = std::array<float, 2>{ {-max_val, -max_val}};
	constexpr auto kXMax = std::array<float, 2>{ {max_val, max_val}};
	const std::uint32_t max_sample_attempts = 30;
	std::vector<std::array<float, 2>> point_data = thinks::PoissonDiskSampling(kRadius, kXMin, kXMax,max_sample_attempts);
	// Samples returned as std::vector<std::array<float, 2>>.
	// Default seed and max sample attempts.
	return point_data;
}
//vertices coordinates
void Print_Verts(std::vector<float>& vertices) {
	
	unsigned int index = 0;
	std::cout << "Beep Boop, Printing....." << '\n';
	for (auto i :vertices)
	{

		index++;
		if ((index % 3) != 0) {
			std::cout << i << ',';
		}
		else
		{

			std::cout << i << ',' << '\n';
		};

	}

}
/*generates a Barycentric Dual Mesh (A kind of Centroidal Voronoi Tesselation)*/
//wip

int main()
{
	auto start = std::chrono::system_clock::now();
	// Initialize GLFW
	glfwInit();
	
	//Generate Poisson Points
	const std::vector<std::array<float, 2>> points = generate_points();
	std::vector<double> coords;
	for (auto point : points) {
		for (auto i : point) {
			coords.push_back((double)i);
		}
	}
	delaunator::Delaunator del_triangles(coords);
	//std::cout << del_triangles.triangles.size() << std::endl;
	//std::cout << del_triangles.triangles.size()<< std::endl;
	std::vector<float> vertices;
	Init_Vertices(del_triangles, vertices);
	std::cout << vertices.size() << '\n';
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 1200 by 1000 pixels, naming it "ImGui + GLFW"
	GLFWwindow* window = glfwCreateWindow(1200, 1000, "ImGui + GLFW", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 1000, y = 1000
	glViewport(0, 0, 1000, 1000);



	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Print_Verts(vertices);
	
	
	// Create reference containers for the Vertex Array Object and the Vertex Buffer Object

	GLuint VAO,VBO;
	//Generate VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	// Variables to be changed in the ImGUI window
	static int generation_stage = 0;
	bool wireframe = false;
	float size = 1.0f;
	float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
	int test = 0;
	// Exporting variables to shaders
	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
	glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);
	// Main while loop

	while (!glfwWindowShouldClose(window))
	{
		/*if (test%20 != 0)
			float color[4] = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };*/
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Only draw the dots if the ImGUI checkbox is ticked
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (generation_stage == 1 || generation_stage == 2)
		{	
			// Draw Triangles
			glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
			if (test == 0)
			{
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;
				std::time_t end_time = std::chrono::system_clock::to_time_t(end);
				std::cout <<"Drawing "<< vertices.size() / 18 << " triangles from " << vertices.size()/12 << " total vertices generated from "<< points.size()<< " points and rendered to view in " << elapsed_seconds.count() << '/n' << " at a rate of " << (vertices.size() /12) / elapsed_seconds.count() << " vertices rendered per second.";
				test++;
			}
			test++;
		};

		if (generation_stage == 0)
		{
			// Draw points
			glDrawArrays(GL_POINTS, 0, vertices.size() / 2);
			glPointSize(1);
		};
		// ImGUI window creation
		ImGui::Begin("My name is window, ImGUI window");
		// Text that appears in the window
		//Button to regenerate points
		ImGui::Checkbox("Wireframe", &wireframe);
		// Checkbox that appears in the window
	
		ImGui::RadioButton("Points", &generation_stage, 0); ImGui::SameLine();
		ImGui::RadioButton("Triangles", &generation_stage, 1); ImGui::SameLine();
		ImGui::RadioButton("Polygons", &generation_stage, 2);
		// Slider that appears in the window
		ImGui::SliderFloat("Size", &size, 0.0001f, 20.0f);
		// Fancy color editor that appears in the window
		ImGui::ColorEdit4("Color", color);
		// Ends the window
		ImGui::End();

		// Export variables to shader
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void Init_Vertices(const delaunator::Delaunator& del_triangles, std::vector<float>& vertices)
{
	//note : replacing del_triangles.triangles.size() with del_triangles.coords.size() makes it a circle, but it looks kinda cool
	for (std::size_t i = 0; i < del_triangles.triangles.size(); i += 3) {
		//Verts 0

		vertices.push_back((float)del_triangles.coords[2 * del_triangles.triangles[i]]);//tx0
		vertices.push_back((float)del_triangles.coords[2 * del_triangles.triangles[i] + 1]);//ty0
		vertices.push_back(0.0);														 //z0

																						 //Colours 0
		vertices.push_back((float)(sin(rand() % 100) + 0.2));
		vertices.push_back(0.f);
		vertices.push_back(0.f);
		//Verts 1
		vertices.push_back((float)del_triangles.coords[2 * del_triangles.triangles[i + 1]]); //tx1
		vertices.push_back((float)del_triangles.coords[2 * del_triangles.triangles[i + 1] + 1]);//ty1
		vertices.push_back(0.0);																//z 1
																								//Colours 1
		vertices.push_back(0.f);
		vertices.push_back((float)(sin(rand() % 100) + 0.2));
		vertices.push_back(0.f);
		//Verts 2
		vertices.push_back((float)del_triangles.coords[2 * del_triangles.triangles[i + 2]]);  //tx2
		vertices.push_back((float)del_triangles.coords[2 * del_triangles.triangles[i + 2] + 1]); //ty2
		vertices.push_back(0.0);														//z	2	
																						//Colours 2

		vertices.push_back(0.f);
		vertices.push_back(0.f);
		vertices.push_back((float)(sin(rand() % 100) + 0.2));
	}
}
