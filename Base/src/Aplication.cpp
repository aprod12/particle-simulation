#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include "VeretexBufferLayout.h"
#include "Vec4.h"


# define M_PI           3.14159265358979323846


void MakeQuads(float quads_index) {
	glTexCoord3f(0.0f, 0.0f, ((float)quads_index + 1.0f) / 2.0f);
	glVertex3f(-1.0f, -1.0f, quads_index);
	glTexCoord3f(1.0f, 0.0f, ((float)quads_index + 1.0f) / 2.0f);
	glVertex3f(1.0f, -1.0f, quads_index);
	glTexCoord3f(1.0f, 1.0f, ((float)quads_index + 1.0f) / 2.0f);
	glVertex3f(1.0f, 1.0f, quads_index);
	glTexCoord3f(0.0f, 1.0f, ((float)quads_index + 1.0f) / 2.0f);
	glVertex3f(-1.0f, 1.0f, quads_index);

}

void Render(unsigned int& tex_d, unsigned int& id) {
	float fFrameCount = tex_d;
	GLCall(glEnable(GL_ALPHA_TEST));
	GLCall(glAlphaFunc(GL_GREATER, 0.03f));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	{
		double matrix[16];
		double rotate[3];
		matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
		matrix[1] = matrix[2] = matrix[3] = matrix[4] =
			matrix[6] = matrix[7] = matrix[8] = matrix[9] =
			matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0;
		rotate[0] = 0.0;
		rotate[1] = 0.0;
		rotate[2] = 0.0;

		GLCall(glMatrixMode(GL_TEXTURE));
		GLCall(glLoadIdentity());
		GLCall(glTranslatef(0.5f, 0.5f, 0.5f));
		GLCall(glMatrixMode(GL_MODELVIEW));
		GLCall(glLoadIdentity());
		GLCall(glRotated(rotate[0], 1.0f, 0, 0));
		GLCall(glRotated(rotate[1], 0, 1.0f, 0));
		GLCall(glRotated(rotate[2], 0, 0, 1.0f));
		GLCall(glMultMatrixd(matrix));
		GLCall(glMatrixMode(GL_TEXTURE));
		GLCall(glTranslatef(-0.5f, -0.5f, -0.5f));
		GLCall(glEnable(GL_TEXTURE_3D));

	}

	for (float fIndx = -1.0f; fIndx <= 1.0f; fIndx += 0.01f) {
		glBegin(GL_QUADS);
		MakeQuads(fIndx);
		glEnd();
	}
	GLCall(glBindTexture(GL_TEXTURE_3D, id));
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
		std::cout << "Baj van";

	std::cout << glGetString(GL_VERSION) << std::endl;


	//dimensions of the image
	unsigned int tex_w = 200, tex_h = 200, text_d = 200;
	unsigned int tex_output;
	GLCall(glGenTextures(1, &tex_output));
	GLCall(glBindTexture(GL_TEXTURE_3D, tex_output));
	GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	GLCall(glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, tex_w, tex_h, text_d, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glGenerateTextureMipmap(tex_output));
	GLCall(glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));

	{
		Shader shaders = Shader("res/shaders/Shader.shader", "res/shaders/Compute.shader");
		std::vector<vec4> circle;



		VertexArray va;
		VertexBuffer vb(circle.data(), 1000 * 4 * sizeof(float));
		VertexBufferLayout layout;
		//layout.Push<float>(4);
		va.AddBuffer(vb, layout);
		Renderer renderer;
		IndexBuffer ib;


		float dt = 1;
		size_t size = 1048576;// 8388608;
		vec4* particlePos = new vec4[size];
		for (int i = 0; i < size; i++)
		{
			float theta = 2 * M_PI * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float phi = acos((2 * v) - 1);
			float random1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r = pow(random1, 1 / 3);
			r *= (tex_w / 3);
			float x = (tex_w / 2) + r * sin(phi) * cos(theta);
			float y = (tex_h / 2) + r * sin(phi) * sin(theta);
			float z = (text_d / 2) + r * cos(phi);
			particlePos[i].x = x;
			particlePos[i].y = y;
			particlePos[i].z = z;

		}
		unsigned int posBuffer;
		GLCall(glGenBuffers(1, &posBuffer));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, posBuffer));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, (size) * sizeof(vec4), particlePos, GL_STATIC_DRAW)); //sizeof(data) only works for statically sized C/C++ arrays.
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, posBuffer));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));


		vec4* particleVel = new vec4[size];
		for (size_t i = 0; i < size; i++)
		{
			particleVel[i].x = rand() % 2 == 1 ? 1 : -1;
			particleVel[i].y = rand() % 2 == 1 ? 1 : -1;
			particleVel[i].z = rand() % 2 == 1 ? 1 : -1;
		}

		unsigned int velBuffer;
		GLCall(glGenBuffers(1, &velBuffer));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBuffer));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(vec4), particleVel, GL_STATIC_DRAW)); //sizeof(data) only works for statically sized C/C++ arrays.
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, velBuffer));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));



		unsigned int bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		// drawing loop
		while (!glfwWindowShouldClose(window)) {

			{ // launch compute shaders!
				shaders.BindCompute();
				shaders.SetUniform1f("dt", dt);
				renderer.Compute(shaders, size / 256, 1, 1);
			}

			{ // normal drawing pass
				renderer.Clear();
				//shaders.Bind();
				//GLCall(glBindBuffer(GL_ARRAY_BUFFER, posBuffer));
				//GLCall(glVertexPointer(4, GL_FLOAT, 0, (void*)0));
				//GLCall(glEnableClientState(GL_VERTEX_ARRAY));
				//GLCall(glDrawArrays(GL_POINTS, 0, size));
				//GLCall(glDisableClientState(GL_VERTEX_ARRAY));
				//GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				Render(text_d, tex_output);
			}

			glfwPollEvents();
			if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
				glfwSetWindowShouldClose(window, 1);
			}
			glfwSwapBuffers(window);
		}
		delete[] particlePos, particleVel;
	}
	glfwTerminate();
	return 0;
}