

#include "WindowApplication.h"
#include "QHelper.h"

GLFWwindow* WindowApplication::s_Window = nullptr;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void WindowApplication::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void WindowApplication::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



GLFWwindow* WindowApplication::CreateWindowAndInit()
{
	//throw std::logic_error("The method or operation is not implemented.");

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	ThrowIfFailed(window);

	return window;
}

void WindowApplication::UpdateApplication(QGameApp& myGameApp)
{
	//throw std::logic_error("The method or operation is not implemented.");

	
}

int WindowApplication::RunApplication(QGameApp& myGameApp)
{

	// 创建窗口
	WindowApplication::s_Window = CreateWindowAndInit();

	
	glfwMakeContextCurrent(s_Window);
	glfwSetFramebufferSizeCallback(s_Window, WindowApplication::FramebufferSizeCallback);

	myGameApp.Init();

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 消息循环
	// render loop
	// -----------
	while (!glfwWindowShouldClose(s_Window))
	{
		// input
		// -----
		WindowApplication::ProcessInput(s_Window);

		//myGameApp.Update();
		//UpdateApplication(myGameApp);
		myGameApp.RenderScene();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(s_Window);
		glfwPollEvents();
	}


	//销毁
	myGameApp.ShutDown();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	

	



	return 0;
}
