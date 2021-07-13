

#include "WindowApplication.h"
#include "QHelper.h"
#include "QCamera.h"
#include "QTimer.h"

GLFWwindow* WindowApplication::s_Window = nullptr;
bool WindowApplication::s_IsFirstTimeMouseIn = true;
float WindowApplication::s_MouseLastX = 0.0f;
float WindowApplication::s_MouseLastY = 0.0f;
bool WindowApplication::s_IsMouseButtonDown = false;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void WindowApplication::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float deltaTime = QTimer::GetInstance().GetDeltaTime();
	const float cameraMoveSpeed = 0.01f;
	QCamera& camera = QCamera::GetInstance();
	float deltaPos = cameraMoveSpeed * deltaTime;
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
	{
		camera.Walk(deltaPos);
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
	{
		camera.Walk( -deltaPos);
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D ))
	{
		//camera.SetPositionAnRefreshViewMat(camera.GetPosition() - camera.GetRight() * cameraMoveSpeed * deltaTime);
		camera.Strafe(deltaPos);

		
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
	{
		camera.Strafe( -deltaPos);
		//camera.SetPositionAnRefreshViewMat(camera.GetPosition() + camera.GetRight() * cameraMoveSpeed * deltaTime);
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Q))
	{
		camera.MoveUp(deltaPos);

		//std::cout << "Camera up = " << camera.GetUp() << std::endl;
		//std::cout << "Camera position = " << camera.GetPosition() << std::endl;
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_E))
	{
		camera.MoveUp(-deltaPos);

	}

	//std::cout << "Camera position =( " << camera.GetPosition().x << ","
	//	<< camera.GetPosition().y  << ", "  << camera.GetPosition().z << ")" << std::endl;
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R))
	{
		camera.SetToDefaultCamera();
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void WindowApplication::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



void WindowApplication::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		return;
	}
	// TODO 这里有一个问题，mouse不仅需要按下，还需要已经持续一段时间，或者说按住，才往后进行
	/*if (!s_IsMouseButtonDown)
	{
		return;
	}*/
	if (s_IsFirstTimeMouseIn)
	{
		s_MouseLastX = xpos;
		s_MouseLastY = ypos;
		s_IsFirstTimeMouseIn = false;
	}
	

	const float sensitivity = 0.1f;
	QCamera& camera = QCamera::GetInstance();
	float mouseMoveX = (xpos - s_MouseLastX);
	float mouseMoveY = (ypos - s_MouseLastY);
	float deltaPitch = mouseMoveY   * sensitivity;
	float deltaYaw = mouseMoveX * sensitivity;
	
	bool test1 = abs(mouseMoveX) < 1.0f;
	bool test2 = abs(mouseMoveY) < 1.0f;
	bool test3 = test1 && test2;
	// 如果deltaPitch/deltaYaw 太大，可能造成闪移，所以忽略
	if ( test3);
	{
		camera.AddPitch(deltaPitch);
		camera.AddYaw(deltaYaw);
	}
	

	s_MouseLastX = xpos;
	s_MouseLastY = ypos;
}

void WindowApplication::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

/*! @brief The function pointer type for mouse button callbacks.
 *
 *  This is the function pointer type for mouse button callback functions.
 *  A mouse button callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int button, int action, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] button The [mouse button](@ref buttons) that was pressed or
 *  released.
 *  @param[in] action One of `GLFW_PRESS` or `GLFW_RELEASE`.  Future releases
 *  may add more actions.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_mouse_button
 *  @sa @ref glfwSetMouseButtonCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle and modifier mask parameters.
 *
 *  @ingroup input
 */
void myMouseButton_callback(GLFWwindow* window, int buttonType, int pressOrRelease, int mods)
{
	if (GLFW_MOUSE_BUTTON_LEFT == buttonType) 
	{
		if (GLFW_PRESS == pressOrRelease)
		{
			WindowApplication::s_IsMouseButtonDown = true;
		}
		else if (GLFW_RELEASE == pressOrRelease)
		{
			WindowApplication::s_IsMouseButtonDown = false;
		}
	}
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

	// setup callbacks
	glfwSetFramebufferSizeCallback(s_Window, WindowApplication::FramebufferSizeCallback);
	glfwSetCursorPosCallback(s_Window, mouse_callback);
	glfwSetScrollCallback(s_Window, scroll_callback);
	glfwSetMouseButtonCallback(s_Window, myMouseButton_callback);

	myGameApp.Init();

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 消息循环
	// render loop
	// -----------
	while (!glfwWindowShouldClose(s_Window))
	{
		QTimer::GetInstance().Update();

		// input
		// -----
		WindowApplication::ProcessInput(s_Window);

		//myGameApp.Update();
		//UpdateApplication(myGameApp);
		myGameApp.Update(QTimer::GetInstance().GetDeltaTime());
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
