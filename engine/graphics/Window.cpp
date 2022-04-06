#include "Window.h"

#include <glew/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>


#include "core/Logger.h"


Window::Window() : width(0),height(0),window(nullptr)
{
}

Window::~Window() 
{
	OnDestroy();
}

void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

bool Window::OnCreate(const char* name, int w, int h)
{


	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
	{
		EngineLogger::Error("SDL_Init went wrong", "window.cpp", __LINE__);
		return false;
	}

	this->width = w;
	this->height = h;


	
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE);

	if (window == nullptr)
	{
		EngineLogger::Error("window is null", "window.cpp", __LINE__);
		return false;
	}


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	context = SDL_GL_CreateContext(window);
	
	int major,minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major); 
	
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	SetAttributes(major, minor);

	


	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		EngineLogger::Error("Glew didnt init", "window.cpp", __LINE__);
		return false;
	}
	glViewport(0, 0, width, height);

	glEnable              ( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( MessageCallback, 0 );

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
															  // Setup Dear ImGui style



	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.06f, 0.1f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.06f, 0.1f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	
	ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

	ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);

	

	ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.48f, 0.76f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.48f, 0.76f, 1.0f);

	ImGui::GetStyle().Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.68f, 0.96f, 1.0f);

	ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.18f, 0.36f, 1.0f);
	ImGui::GetStyle().Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.18f, 0.36f, 1.0f);


	

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 450");

	return true;
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
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
	
}