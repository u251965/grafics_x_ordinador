/*
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include <vector>
#include "button.h"   


class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	int borderWidth = 1;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	enum Mode { MODE_PAINT, MODE_ANIM };
	enum Tool { TOOL_PENCIL, TOOL_ERASER, TOOL_LINE, TOOL_RECT, TOOL_TRI };

	Mode mode = MODE_PAINT;
	Tool currentTool = TOOL_PENCIL;

	bool fillShapes = false;

	Color currentColor = Color::WHITE;

	Image canvas;

	bool isDragging = false;
	Vector2 startPos;
	Vector2 lastPos;
	Vector2 currentPos;

	std::vector<Button> buttons;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init(void);
	void Render(void);
	void Update(float dt);



	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport(0, 0, width, height);
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
	}

	Vector2 GetWindowSize()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return Vector2(float(w), float(h));
	}
};
