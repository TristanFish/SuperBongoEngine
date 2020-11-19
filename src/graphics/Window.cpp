#include "Window.h"


Window::Window() : window(nullptr)
{
}

Window::~Window() 
{
	OnDestroy();
}

void Window::OnCreate(const char* name, int w, int h)
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
	{
		std::cout << "SDL_INIT_EVERYTHING went wrong in Window" << std::endl;
	}

	this->width = w;
	this->height = h;

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	
	if (window == nullptr)
	{
		std::cout << "Window is null" << std::endl;
	}

	context = SDL_GL_CreateContext(window);
	int major;
	int minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	SetAttributes(major, minor);


	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Glew didnt init in Window" << std::endl;
	}
	glViewport(0, 0, width, height);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	
															  // Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void Window::OnDestroy()
{
	// ImGuiCleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

SDL_Window* Window::GetWindow() const
{
	return window;
}

void Window::SetAttributes(int major, int minor)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
}