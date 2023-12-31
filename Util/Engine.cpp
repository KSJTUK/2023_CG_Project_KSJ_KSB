#include "pch.h"
#include "Util/Engine.h"
#include "Util/Input.h"
#include "Sound.h"



#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"

Engine::Engine() { }

Engine::~Engine() { }

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


void Engine::Init() {
	// 이 프로그램이 1번 코어에서만 작동하도록 변경 
	// 이 설정이 없으면 멀티코어 실행환경에서 타이머의 Frequency 를 얻어오는 프로세서와 
	// 실제로 Update 를 수행하는 프로세서가 달라 오차가 생길 수 있다.
	SetThreadAffinityMask(GetCurrentProcess(), 1);

	// glfw 라이브러리 초기화
	if (!glfwInit()) {
		exit(-1);
	}

	// OpenGL 버전 설정
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 윈도우 정보 초기 설정
	m_windowInfo.width = 1920;
	m_windowInfo.height = 1080;
	m_windowInfo.x = 0;
	m_windowInfo.y = 0;
	m_windowInfo.windowTitle = "FPS: ";

	// 윈도우 생성
	m_windowInfo.window = glfwCreateWindow(m_windowInfo.width, m_windowInfo.height, m_windowInfo.windowTitle.c_str(), NULL, NULL);
	if (m_windowInfo.window == nullptr) {
		glfwTerminate();
		exit(-1);
	}

	// 생성한 윈도우를 현재 메인 윈도우로 설정
	glfwMakeContextCurrent(m_windowInfo.window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		exit(-1);
	}

	glfwSetInputMode(m_windowInfo.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 깊이 테스트 설정
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// 뷰포트 설정
	glViewport(0, 0, m_windowInfo.width, m_windowInfo.height);

	glfwSwapInterval(m_swapInterver);

	Input::GetInstance()->Init(m_windowInfo.window);
	Sound::GetInstance()->Init();


	Sound::GetInstance()->NewSound(std::string("Gun"), std::string("Sound/gun.wav"),FMOD_DEFAULT);
	Sound::GetInstance()->NewSound(std::string("zombie_idle"), std::string("Sound/zombie_idle.wav"), FMOD_DEFAULT);
	Sound::GetInstance()->NewSound(std::string("zombie_hit"), std::string("Sound/zombie_hit.wav"), FMOD_DEFAULT);


	SHADER->Initialize();

	m_timer = std::make_unique<Timer>();
	m_renderer = std::make_unique<Renderer>(m_windowInfo.window);

	Sound::GetInstance()->Update();


	Sound::GetInstance()->Play(std::string("Gun"),0);


	std::cout << "\nThis Program Use This GPU : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "======================================END INITIALIZATION======================================" << std::endl;
}


void Engine::Update() {
	// 게임 업데이트 함수
	m_timer->Update();
	glfwSetWindowTitle(m_windowInfo.window, std::string(m_windowInfo.windowTitle + std::to_string(m_timer->GetFps())).c_str());
	Input::GetInstance()->Update();
	Sound::GetInstance()->Update();

	m_deltaTime = m_timer->GetDeltaTime();

	m_renderer->Update(m_deltaTime);
}

void Engine::LateUpdate() {
	// 게임 업데이트를 호출하고
	// 추가로 더 업데이트를 해줘야 할 일이 생기면 이곳에 작성
	// 업데이트와 순서를 명확히 구분하기 위해 쓰이는 함수 ex) 오브젝트 삭제
}

void Engine::Render() {
	//// 렌더링 코드
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_renderer->Render();

	glfwSwapBuffers(m_windowInfo.window);
}

void Engine::Loop() {
	while (!glfwWindowShouldClose(m_windowInfo.window)) {
		Update();

		LateUpdate();

		Render();


		glfwPollEvents();
	}
}
