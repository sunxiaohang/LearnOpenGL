#include "common/Utils.h"
#include "common/Shader.h"
#include "common/Renderer.h"
#include "common/VertexBuffer.h"
#include "common/IndexBuffer.h"
#include "common/VertexArray.h"
#include "common/VertexBufferLayout.h"
#include "common/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void RendererInternal(const Renderer& renderer, const VertexArray& vertexArray, const IndexBuffer& indexBuffer, Shader& shader);

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float vertices[] = {
		 0.5f,  0.5f, 1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		1, 3, 2   // second Triangle
	};
	GLTryCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glEnable(GL_BLEND);
	VertexArray vertexArray;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	VertexBuffer vertexBuffer(vertices, 4 * 4 * sizeof(float));
	IndexBuffer indexBuffer(indices, 2 * 3);
	vertexArray.AddBuffer(vertexBuffer, layout);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	Renderer renderer;
	Shader shader("BaseShader.shader");
	Texture texture("github.png");
	texture.Bind();

	glfwSwapInterval(1);
	//ASSERT(location != -1);
	while (!glfwWindowShouldClose(window))// render loop
	{
		processInput(window);// input
		renderer.Clear();
		RendererInternal(renderer, vertexArray, indexBuffer, shader);
		glfwSwapBuffers(window);// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	vertexArray.UnBind();
	vertexBuffer.UnBind();
	indexBuffer.UnBind();
	shader.UnBind();
	glfwTerminate();// glfw: terminate, clearing all previously allocated GLFW resources.
	return 0;
}

void RendererInternal(const Renderer& renderer, const VertexArray& vertexArray, const IndexBuffer& indexBuffer, Shader& shader)
{
	//variable init
	glm::vec3 translationA = glm::vec3(0, 0, 0);
	glm::vec3 translationB = glm::vec3(0, 0, 0);

	//processing logic
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::SliderFloat3("translationA", &translationA.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
	ImGui::SliderFloat3("translationB", &translationB.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	{
		glm::mat4 modelA = glm::translate(glm::mat4(1.0f), translationA);
		glm::mat4 mvpA = proj * view * modelA;
		shader.Bind();
		shader.SetUniformMat4("projection", mvpA);
		renderer.Draw(vertexArray, indexBuffer, shader);
	}

	{
		glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
		glm::mat4 mvpB = proj * view * modelB;
		shader.Bind();
		shader.SetUniformMat4("projection", mvpB);
		renderer.Draw(vertexArray, indexBuffer, shader);
	}
	shader.SetUniform1i("u_Texture", 0);

	//postprocess
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}