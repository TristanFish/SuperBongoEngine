#ifndef WINDOW_H
#define WINDOW_H

#include <glew/glew.h>
#include <sdl/SDL.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

//! Window Class
/*! Controls the windows attributes such as width & height */
class Window
{
private:
	//! Sizing variables
	/*! Stores the size of the opened window */
	int width, height;

	//! SDL Window pointer
	/*! Pointer to a new Sdl_Window that we use */
	SDL_Window* window;

	//! SDL/OpenGL context
	/*! Is the context that is used for the OpenGL window */
	SDL_GLContext context;

	//! Set Attributes setter
	/*! Is used to set the SDL/OpenGl attributes */
	void SetAttributes(int major, int minor);


public:

	//! Window Constructor
	/*! Empty Function */
	Window();

	//! Window Destructor
	/*! Calls the on Destroy function */
	~Window();

	//! OnCreate Function 
	/*! Initialized all the needed variables */
	void OnCreate(const char* name, int w, int h);

	//! OnDestroy Function 
	/*! Calls the windows destroy functions and deletes any needed pointers */
	void OnDestroy();

	//! GetWidth Getter 
	/*! Returns the screens width  */
	int GetWidth() const;

	//! GetHeight Getter 
	/*! Returns the screens height  */
	int GetHeight() const;

	//! GetWindow Getter 
	/*! Returns the window pointer  */
	SDL_Window* GetWindow() const;
	
};

#endif