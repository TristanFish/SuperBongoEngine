#ifndef WINDOW_H
#define WINDOW_H

#include <glew/glew.h>
#include <sdl/SDL.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

class Window
{
private:
	int width, height;
	SDL_Window* window;
	SDL_GLContext context;
	void SetAttributes(int major, int minor);


public:
	Window();
	~Window();

	void OnCreate(const char* name, int w, int h);
	void OnDestroy();

	int GetWidth() const;
	int GetHeight() const;
	SDL_Window* GetWindow() const;
	

	


};

#endif