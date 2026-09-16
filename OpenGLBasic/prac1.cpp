#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <random>

using namespace std;

GLFWwindow* window = nullptr;

bool timerStarted = false;

std::random_device rd{};
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);

void keyCallback(GLFWwindow*, int, int, int, int);

void update();
void DrawScene();

int main() 
{
	//--- GLFW 초기화
	if (!glfwInit()) 
	{
		std::cerr << "GLFW 초기화 실패!" << std::endl;
		return -1;
	}

	//--- OpenGL 버전 설정(예: 3.3 Core Profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//--- 윈도우 생성
	window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	
	if (!window) 
	{
		std::cerr << "윈도우 생성 실패!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//--- 컨텍스트 설정
	glfwMakeContextCurrent(window);

	//--- GLEW 초기화
	glewExperimental = GL_TRUE; // 최신 기능 사용
	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "GLEW 초기화 실패!" << std::endl;
		return -1;
	}

	//--- 뷰포트 설정
	glViewport(0, 0, 800, 600);

	// 키 입력 콜백 함수 등록
	glfwSetKeyCallback(window, keyCallback);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	//--- 메인 루프
	while (!glfwWindowShouldClose(window)) 
	{

		update();

		// 화면 지우기 (파란색)
		DrawScene();
		// 버퍼 교체
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//--- 종료 처리
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//--- 키보드 입력 처리 함수
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_C:
			glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
			break;
		case GLFW_KEY_M:
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			break;
		case GLFW_KEY_Y:
			glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_A:
			glClearColor(dis(gen), dis(gen), dis(gen), 1.0f);
			break;
		case GLFW_KEY_G:
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			break;
		case GLFW_KEY_K:
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		case GLFW_KEY_T:
			timerStarted = true;
			break;
		case GLFW_KEY_S:
			timerStarted = false;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		}
	}
}

void update()
{
	if (timerStarted && glfwGetTime() >= 1.0f)
	{
		glClearColor(dis(gen), dis(gen), dis(gen), 1.0f);
		glfwSetTime(0.0);
	}
}

//--- 렌더링 함수
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
