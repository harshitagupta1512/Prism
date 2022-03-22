#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <iostream>
#include <camera.h>

#define M_PI 3.14159265358979323846

typedef struct vertex
{
	float x;
	float y;
	float z;
} vertex;

typedef struct triangle
{
	int v1;
	int v2;
	int v3;

} triangle;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

//camera position is (1, 1, 10) in wc
//other arguments in constructor use their default values
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

float angle = 0.0;

bool isR = false;
bool turn_table = false;
int d = 0;

glm::mat4 transform = glm::mat4(1.0f);
//transform matrix initialised as identity matrix

glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

void toggle_key(GLFWwindow *window, int key, int scancode, int action,
				int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		isR = !isR;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		turn_table = !turn_table;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Invalid input" << std::endl;
		return 0;
	}
	int n = atoi(argv[1]);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PART_A", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	vertex polygon1[n + 1]; //centre + n vertices
	polygon1[0] = {0.0, 0.0, -0.25};
	for (int i = 1; i <= n; i++)
	{
		polygon1[i].x = cos((2 * M_PI * i) / n) * 0.5;
		polygon1[i].y = sin((2 * M_PI * i) / n) * 0.5;
		polygon1[i].z = -0.25;
	}

	//draw n triangles - polygon - 3n vertices-3 coordinates + 3colors

	vertex polygon2[n + 1]; //centre + n vertices
	polygon2[0] = {0.0, 0.0, 0.25};

	for (int i = 1; i <= n; i++)
	{
		polygon2[i].x = cos((2 * M_PI * i) / n) * 0.5;
		polygon2[i].y = sin((2 * M_PI * i) / n) * 0.5;
		polygon2[i].z = 0.25;
	}

	triangle T[n];
	for (int i = 0; i < n - 1; i++)
	{
		T[i].v1 = 0;
		T[i].v2 = i + 1;
		T[i].v3 = i + 2;
	}
	T[n - 1].v1 = 0;
	T[n - 1].v2 = 1;
	T[n - 1].v3 = n;

	float vertices[3 * n * 6 * 4];

	int itr = 0;

	//2 * n more triangles
	//6n more vertices

	float c1, c2, c3;

	c1 = 0.1f;
	c2 = 0.1f;
	c3 = 0.1f;

	for (int i = 1; i <= n; i++)
	{
		//add a rectangle
		//add 2 triangles in each iteration
		//p1_i, p1_(i+1), p2_i, p2_(i+1) - rectangle
		//add 6 vertices

		int a, b;
		a = i;
		b = i + 1;

		if (i == n)
		{
			a = n;
			b = 1;
		}

		vertices[itr] = polygon1[a].x;
		itr++;
		vertices[itr] = polygon1[a].y;
		itr++;
		vertices[itr] = polygon1[a].z;
		itr++;
		vertices[itr] = c1;
		itr++;
		vertices[itr] = c2;
		itr++;
		vertices[itr] = c3;
		itr++;

		vertices[itr] = polygon1[b].x;
		itr++;
		vertices[itr] = polygon1[b].y;
		itr++;
		vertices[itr] = polygon1[b].z;
		itr++;
		vertices[itr] = c1;
		itr++;
		vertices[itr] = c2;
		itr++;
		vertices[itr] = c3;
		itr++;

		vertices[itr] = polygon2[a].x;
		itr++;
		vertices[itr] = polygon2[a].y;
		itr++;
		vertices[itr] = polygon2[a].z;
		itr++;
		vertices[itr] = c1;
		itr++;
		vertices[itr] = c2;
		itr++;
		vertices[itr] = c3;
		itr++;

		vertices[itr] = polygon2[a].x;
		itr++;
		vertices[itr] = polygon2[a].y;
		itr++;
		vertices[itr] = polygon2[a].z;
		itr++;
		vertices[itr] = c1;
		itr++;
		vertices[itr] = c2;
		itr++;
		vertices[itr] = c3;
		itr++;

		vertices[itr] = polygon2[b].x;
		itr++;
		vertices[itr] = polygon2[b].y;
		itr++;
		vertices[itr] = polygon2[b].z;
		itr++;
		vertices[itr] = c1;
		itr++;
		vertices[itr] = c2;
		itr++;
		vertices[itr] = c3;
		itr++;

		vertices[itr] = polygon1[b].x;
		itr++;
		vertices[itr] = polygon1[b].y;
		itr++;
		vertices[itr] = polygon1[b].z;
		itr++;
		vertices[itr] = c1;
		itr++;
		vertices[itr] = c2;
		itr++;
		vertices[itr] = c3;
		itr++;

		c1 = c1 + 0.015;
		c2 = c2 + 0.015;
		c3 = c3 + 0.015;
	}

	for (int i = 0; i < n; i++)
	{
		//go through the array T
		int v[3];
		v[0] = T[i].v1;
		v[1] = T[i].v2;
		v[2] = T[i].v3;

		//add 3 vertices
		//6 values for each vertex
		//add a total of 18 values

		for (int i = 0; i < 3; i++)
		{
			//for each vertex
			vertices[itr] = polygon2[v[i]].x;
			itr++;
			vertices[itr] = polygon2[v[i]].y;
			itr++;
			vertices[itr] = polygon2[v[i]].z;
			itr++;
			vertices[itr] = 1.0f;
			itr++;
			vertices[itr] = 0.8f;
			itr++;
			vertices[itr] = 0.0f;
			itr++;
		}
	}
	for (int i = 0; i < n; i++)
	{
		//go through the array T
		int v[3];
		v[0] = T[i].v1;
		v[1] = T[i].v2;
		v[2] = T[i].v3;

		//add 3 vertices
		//6 values for each vertex
		//add a total of 18 values

		for (int i = 0; i < 3; i++)
		{
			//for each vertex
			vertices[itr] = polygon1[v[i]].x;
			itr++;
			vertices[itr] = polygon1[v[i]].y;
			itr++;
			vertices[itr] = polygon1[v[i]].z;
			itr++;
			vertices[itr] = 0.0f;
			itr++;
			vertices[itr] = 0.8f;
			itr++;
			vertices[itr] = 1.0f;
			itr++;
		}
	}
	std::cout << itr << std::endl;

	for (int i = 0; i < itr; i++)
	{
		std::cout << vertices[i] << " ";
		i++;
		std::cout << vertices[i] << " ";
		i++;
		std::cout << vertices[i] << " ";
		i++;
		std::cout << vertices[i] << " ";
		i++;
		std::cout << vertices[i] << " ";
		i++;
		std::cout << vertices[i] << " ";
		std::cout << std::endl;
	}

	//create the vertices array

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// render loop
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ourShader.use();
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// input
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glfwSetKeyCallback(window, toggle_key);

		// render
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		// get matrix's uniform location and set matrix
		// pass projection matrix to shader (note that in this case it could change every frame)

		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// make sure to initialize matrix to identity matrix first
		//transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));

		transform = glm::mat4(1.0f);

		transform = glm::translate(transform, pos);

		if (isR)
		{
			//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
			angle += (float)(deltaTime * 60.0f);
		}

		transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

		//else
		//	transform = glm::mat4(1.0f);

		ourShader.setMat4("transform", transform);
		//argument - target

		glm::mat4 view = camera.GetViewMatrix2(glm::vec3(0.0f, 0.0f, 0.0f));
		ourShader.setMat4("view", view);

		//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		if (turn_table)
		{
			if (d)
			{
				camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
				pos = glm::vec3(0.0f, 0.0f, 0.0f);
				d = 0;
			}
			float w = 100.0f * deltaTime;
			glm::mat4 rot = glm::mat3(1.0f);
			rot = glm::rotate(rot, glm::radians(w), glm::vec3(0.0f, 1.0f, 0.0f));
			camera.changePosition(glm::vec3(rot * glm::vec4(camera.Position, 1.0f)));
		}
		// render container
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 12 * n);

		//4n triangles
		//12n vertices

		//number of vertices to render = n+1
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	/* Flying Camera - W and S for forward and backward, A and D for left and right and Q and E for up and down
	for the respective camera movement. Ensure the camera faces the prism’s (center) at all times */
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			angle = 0.0f;
			d = 0;
		}
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.ProcessKeyboard(UPDIR, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.ProcessKeyboard(DOWNDIR, deltaTime);
	}
	/*
	Object Translation - Another six keys of your choice should be assigned for 6D (up,down, left, right, towards, away
	w.r.t to the camera) movement of the prism. The camera need not follow the prism.
	*/
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		pos = pos + glm ::vec3(0.0f, 0.05f, 0.0f);
		d = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		pos = pos + glm ::vec3(0.0f, -0.05f, 0.0f);
		d = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		pos = pos + glm ::vec3(-0.05f, 0.00f, 0.0f);
		d = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		pos = pos + glm::vec3(0.05f, 0.0f, 0.0f);
		d = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		pos = pos + glm ::vec3(0.0f, 0.0f, 0.05f);
		d = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		pos = pos + glm ::vec3(0.0f, 0.0f, -0.05f);
		d = 1;
	}
	/*Prism fashion show - Assign 2 numeric keys to move the camera to any pre-defined position of choice.
	But, ensure the camera is still facing the prism*/

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.changePosition(glm::vec3(3.0f, 1.0f, 5.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		if (d)
		{
			camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
			pos = glm::vec3(0.0f, 0.0f, 0.0f);
			d = 0;
			angle = 0.0f;
		}
		camera.changePosition(glm::vec3(1.0f, 5.0f, 3.0f));
	}
	/*Prism, let’s go for a spin - On press of the key R, the prism should rotate about any single axis (X, Y, Z) of your choice*/

	// if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	// {
	// 	float w = 50.0f * deltaTime;
	// 	angle += w;
	// }

	/*Oh! How the turntables? - The turntable effect involves making the camera revolve around the object of interest.
	Implement this effect by making the camera rotate around the prism on pressing the key T.
	The camera should always be facing the prism.*/

	// if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	// {

	// }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}