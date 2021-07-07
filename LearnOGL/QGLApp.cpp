#include "QGLApp.h"
#include "MyOGLHelp.h"

const char* g_VertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* g_FragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

QGLApp::QGLApp() : m_glWindow(nullptr)
{

}



bool QGLApp::Init()
{
	//init  Window
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//glfw window creation
		m_glWindow = glfwCreateWindow(800, 600, "QGLApp", nullptr, nullptr);
		if (nullptr == m_glWindow)
		{
			std::cout << "failed to create gl window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(m_glWindow);
		glfwSetFramebufferSizeCallback(m_glWindow, QGLApp::QGLAppResizeCallBack);
		
	}

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}


	return true;
}

int QGLApp::Run()
{
	// build and compile shader program
#pragma region PrepareShaderProgram
	//compile shaders
	unsigned int vertexShaderID = CreateAndCompileShaderWithCheck(GL_VERTEX_SHADER, g_VertexShaderSource);
	unsigned int fragmentShaderID = CreateAndCompileShaderWithCheck(GL_FRAGMENT_SHADER, g_FragmentShaderSource);

	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);
	glLinkProgram(shaderProgram);

	// check for link erros
	int linkSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		char  errorInfoBuffer[512]; //一般错误信息不会超过512
		glGetProgramInfoLog(shaderProgram, 512, nullptr, errorInfoBuffer);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorInfoBuffer << std::endl;
	}

	// delete useless shader object
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
#pragma endregion PrepareShaderProgram
	

	// 配置VBO VAO 将顶点数据上传到GPU
#pragma region PrepareVAO
		// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//先绑定 VAO，再绑定VBO/EBO, 上传数据，在设置vertex attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // bind 0 是类似于unbind的操作， 

	//不要 unbind EBO，因为EBO是存储在VAO中的， keep EBO bound
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//当不在需要更新VAO的数据时unbind ， 注意unbind 并没有释放VAO
	glBindVertexArray(0);
#pragma endregion PrepareVAO


	//GameLoop (Render Loop)
// render loop
	// -----------
	while (!glfwWindowShouldClose(m_glWindow))
	{
		// input
		// -----
		ProcessInput(m_glWindow);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(m_glWindow);
		glfwPollEvents();
	}

	//deallocate resources once they have outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return true;
}




void QGLApp::QGLAppResizeCallBack(GLFWwindow*, int width, int height)
{

	// make sure the viewport matches the new window dimensions; note that width and 
   // height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

bool QGLApp::InitWindow()
{
	//throw std::logic_error("The method or operation is not implemented.");
	return true;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void QGLApp::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

unsigned int QGLApp::CreateAndCompileShaderWithCheck(GLenum shaderType, const char *shaderContent)
{
	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderContent, nullptr);
	glCompileShader(shaderID);

	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char erroInfo[512];
		glGetShaderInfoLog(shaderID, 512, nullptr, erroInfo);
		std::cout << "ERROR::SHADER::COMPILE_FAILED\n" << erroInfo << std::endl;
	}

	return shaderID;
	//throw std::logic_error("The method or operation is not implemented.");
}
