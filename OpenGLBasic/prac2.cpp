#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

random_device rd{};
mt19937 gen(rd());
uniform_real_distribution<float> dis(0.0f, 1.0f);

using namespace std;

GLFWwindow* window{ nullptr };

double mouseX{}, mouseY{};
bool isClicked{};

struct Rect
{
	double x;
	double y;
	double width;
	double height;

	double color[3];

	bool selected;

	Rect() = default;

	Rect(double _x, double _y, double _width, double _height) 
		: x(_x), y(_y), width(_width), height(_height), selected(false) { randomizeColor(); }

	void draw() const
	{
		glColor3f(color[0], color[1], color[2]);
		glRectf(x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0);

		if (selected)
		{
			glColor3f(1.0f - color[0], 1.0f - color[1], 1.0f - color[2]);
			glLineWidth(4.0f);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x - width / 2.0, y - height / 2.0);
			glVertex2f(x + width / 2.0, y - height / 2.0);
			glVertex2f(x + width / 2.0, y + height / 2.0);
			glVertex2f(x - width / 2.0, y + height / 2.0);
			glEnd();
		}
	}

	void scale(double dis)
	{
		width += dis;
		height += dis;

		if (width < 0.1) width = 0.1;
		else if (width > 1.0) width = 1.0;
		if (height < 0.1) height = 0.1;
		else if (height > 1.0) height = 1.0;

	}

	void randomizeColor() { for (int i = 0; i < 3; i++) color[i] = dis(gen); }
};

Rect bgRects[4] =
{
	Rect(0.5, 0.5, 1.0, 1.0),
	Rect(-0.5, 0.5, 1.0, 1.0),
	Rect(-0.5, -0.5, 1.0, 1.0),
	Rect(0.5, -0.5, 1.0, 1.0)
};

vector<vector<Rect>> rects;

void keyCallback(GLFWwindow*, int, int, int, int);

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) //--- 등록한 사용자 정의 콜백 함수
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &mouseX, &mouseY);
		isClicked = true;
	}
}

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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);		// 창 크기 조절 불가

	//--- 윈도우 생성
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Window", nullptr, nullptr);

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
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// 키 입력 콜백 함수 등록
	glfwSetKeyCallback(window, keyCallback);
	
	// 마우스 입력 콜백 함수 등록
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 4; ++i) rects.push_back(vector<Rect>());

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
		case GLFW_KEY_1:
			if (rects[0].size() < 5)
			{
				rects[0].push_back(Rect(0.5f, 0.5f, 1.0f - (rects[0].size() + 1) * 0.1f, 1.0f - (rects[0].size() + 1) * 0.1f));
			}
			break;
		case GLFW_KEY_2:
			if (rects[1].size() < 5)
			{
				rects[1].push_back(Rect(-0.5f, 0.5f, 1.0f - (rects[1].size() + 1) * 0.1f, 1.0f - (rects[1].size() + 1) * 0.1f));
			}
			break;
		case GLFW_KEY_3:
			if (rects[2].size() < 5)
			{
				rects[2].push_back(Rect(-0.5f, -0.5f, 1.0f - (rects[2].size() + 1) * 0.1f, 1.0f - (rects[2].size() + 1) * 0.1f));
			}
			break;
		case GLFW_KEY_4:
			if (rects[3].size() < 5)
			{
				rects[3].push_back(Rect(0.5f, -0.5f, 1.0f - (rects[3].size() + 1) * 0.1f, 1.0f - (rects[3].size() + 1) * 0.1f));
			}
			break;
		case GLFW_KEY_EQUAL:
			for (int i = 0; i < 4; ++i)
			{
				for (auto& rect : rects[i]) if (rect.selected) 
				{
					rect.scale(0.01); 
					break;
				}
			}
			break;
		case GLFW_KEY_MINUS:
			for (int i = 0; i < 4; ++i)
			{
				for (auto& rect : rects[i]) if (rect.selected) 
				{
					rect.scale(-0.01);
					break;
				}
			}
			break;
		case GLFW_KEY_C:
			for (int i = 0; i < 4; ++i)
			{
				for (auto& rect : rects[i]) if (rect.selected)
				{
					rect.randomizeColor();
					break;
				}
			}
			break;
		case GLFW_KEY_R:
			for (int i = 0; i < 4; ++i) bgRects[i].randomizeColor();
			for (auto& rectList : rects) rectList.clear();
			break;
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, true);
			break;
		}
	}
}

void update()
{
	if (!isClicked) return;

	for (int i = 0; i < 4; ++i)
	{
		for (auto& rect : rects[i])
		{
			rect.selected = false;
		}
	}

	bool found = false;

	for (int i = 3; i >= 0; --i)
	{
		if (rects[i].empty()) continue;

		for (int j = rects[i].size() - 1; j >= 0; --j)
		{
			const auto& r = rects[i][j];

			// 사각형의 중앙(r.x, r.y) 기준 최소/최대 NDC 좌표 계산
			double minNdcX = r.x - (r.width / 2.0);
			double maxNdcX = r.x + (r.width / 2.0);

			// OpenGL Y축(위쪽 +) -> 화면 Y축(아래쪽 +) 변환 고려
			double minNdcY = r.y - (r.height / 2.0);
			double maxNdcY = r.y + (r.height / 2.0);

			// NDC 좌표를 마우스 좌표계(Screen 픽셀)로 변환
			double minX = (minNdcX + 1.0) * SCREEN_WIDTH / 2.0;
			double maxX = (maxNdcX + 1.0) * SCREEN_WIDTH / 2.0;

			// Y축은 반전(1.0 - Y)되므로 min/max 변환 위치가 바뀝니다.
			double minY = (1.0 - maxNdcY) * SCREEN_HEIGHT / 2.0;
			double maxY = (1.0 - minNdcY) * SCREEN_HEIGHT / 2.0;

			// 마우스 충돌 검사
			if (mouseX >= minX && mouseX <= maxX && mouseY >= minY && mouseY <= maxY)
			{
				rects[i][j].selected = true;
				found = true;
				break; // inner loop 탈출
			}
		}

		if (found) break; // outer loop 탈출 (단 하나만 선택)
	}

	isClicked = false;
}

//--- 렌더링 함수
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 4; ++i) bgRects[i].draw();
	for (int i = 0; i < 4; ++i)
	{
		for (const auto& rect : rects[i]) rect.draw();
	}
}
