#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include <string>



Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
	this->canvas.Resize(w, h);
	this->canvas.Fill(Color::BLACK);

}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	buttons.clear();

	float y = framebuffer.height - 64.0f;
	float x = 10.0f;
	float step = 70.0f;

	auto addButton = [&](const char* filename, ButtonType type)
		{
			Button b;
			b.icon.LoadPNG(filename, true); 
			b.pos = Vector2(x, y);
			b.type = type;
			buttons.push_back(b);
			x += step;
		};

	// ===== TOOLS =====
	addButton("images/pencil.png", BTN_PENCIL);
	addButton("images/eraser.png", BTN_ERASER);
	addButton("images/line.png", BTN_LINE);
	addButton("images/rectangle.png", BTN_RECT);
	addButton("images/triangle.png", BTN_TRI);

	// ===== COLORS =====
	addButton("images/white.png", BTN_COLOR_WHITE);
	addButton("images/red.png", BTN_COLOR_RED);
	addButton("images/green.png", BTN_COLOR_GREEN);
	addButton("images/blue.png", BTN_COLOR_BLUE);
	addButton("images/yellow.png", BTN_COLOR_YELLOW);
	addButton("images/pink.png", BTN_COLOR_PINK);
	addButton("images/cyan.png", BTN_COLOR_CYAN);
	addButton("images/black.png", BTN_COLOR_BLACK);

	// ===== ACTIONS =====
	addButton("images/clear.png", BTN_CLEAR);
	addButton("images/load.png", BTN_LOAD);
	addButton("images/save.png", BTN_SAVE);
}




// Render one frame

void Application::Render(void)
{
	//framebuffer.Fill(Color::BLUE);
	//framebuffer.DrawLineDDA(500, 500, 30, 20, Color::BLACK);  //proba de linia amb l'algoritme DDA
	//framebuffer.DrawRect(200, 150, 300, 200, Color::WHITE, borderWidth, true, Color::RED );  //proba rectangle

	Vector2 a(500, 300);
	Vector2 b(200, 650);
	Vector2 c(650, 800);

	//framebuffer.DrawTriangle(a, b, c, Color::WHITE, true, Color::RED);  //proba triangle

	// 1) mostrar lienzo
	framebuffer.DrawImage(canvas, 0, 0);

	// 2) preview 
	if (isDragging)
	{
		if (currentTool == TOOL_LINE)
			framebuffer.DrawLineDDA((int)startPos.x, (int)startPos.y, (int)currentPos.x, (int)currentPos.y, currentColor);

		else if (currentTool == TOOL_RECT)
			framebuffer.DrawRect((int)startPos.x, (int)startPos.y,
				(int)(currentPos.x - startPos.x), (int)(currentPos.y - startPos.y),
				currentColor, borderWidth, fillShapes, currentColor);

		else if (currentTool == TOOL_TRI)
		{
			// versión simple: tri con 2 puntos + un tercero fijo (cutre pero funcional)
			Vector2 p0 = startPos;
			Vector2 p1 = currentPos;
			Vector2 p2 = Vector2(startPos.x, currentPos.y);
			framebuffer.DrawTriangle(p0, p1, p2, currentColor, fillShapes, currentColor);
		}
	}

	// 3) toolbar
	for (auto& b : buttons)
		framebuffer.DrawImage(b.icon, (int)b.pos.x, (int)b.pos.y);


	framebuffer.Render();
}


// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event 
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE:
		exit(0);
		break;

	case SDLK_1:
		mode = MODE_PAINT;
		break;

	case SDLK_2:
		mode = MODE_ANIM;
		break;

	case SDLK_f:
		fillShapes = !fillShapes;
		break;

	case SDLK_PLUS:
	case SDLK_KP_PLUS:
		borderWidth++;
		break;

	case SDLK_MINUS:
	case SDLK_KP_MINUS:
		if (borderWidth > 1)
			borderWidth--;
		break;
	}
}


void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button != SDL_BUTTON_LEFT) return;

	// 1) click en botones
	for (auto& b : buttons)
	{
		if (!b.IsMouseInside(mouse_position)) continue;

		switch (b.type)
		{
		case BTN_PENCIL: currentTool = TOOL_PENCIL; return;
		case BTN_ERASER: currentTool = TOOL_ERASER; return;
		case BTN_LINE:   currentTool = TOOL_LINE;   return;
		case BTN_RECT:   currentTool = TOOL_RECT;   return;
		case BTN_TRI:    currentTool = TOOL_TRI;    return;

		case BTN_COLOR_WHITE: currentColor = Color::WHITE; return;
		case BTN_COLOR_RED:   currentColor = Color::RED;   return;
		case BTN_COLOR_GREEN: currentColor = Color::GREEN; return;
		case BTN_COLOR_BLUE:   currentColor = Color::BLUE;   return;
		case BTN_COLOR_YELLOW: currentColor = Color::YELLOW; return;
		case BTN_COLOR_PINK:   currentColor = Color::PURPLE;   return;
		case BTN_COLOR_CYAN:   currentColor = Color::CYAN;   return;


		case BTN_CLEAR: canvas.Fill(Color::BLACK); return;

		case BTN_LOAD:  canvas.LoadPNG("res/images/test.png", true); return; // luego lo haces “bien”
		case BTN_SAVE:  canvas.SaveTGA("my_paint.tga"); return;
		}
	}

	// 2) empezar dibujo
	isDragging = true;
	startPos = mouse_position;
	lastPos = mouse_position;
	currentPos = mouse_position;

	// pencil/eraser: pinta un punto ya
	if (currentTool == TOOL_PENCIL)
		canvas.DrawLineDDA((int)lastPos.x, (int)lastPos.y, (int)lastPos.x, (int)lastPos.y, currentColor);
	if (currentTool == TOOL_ERASER)
		canvas.DrawLineDDA((int)lastPos.x, (int)lastPos.y, (int)lastPos.x, (int)lastPos.y, Color::BLACK);
}


void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button != SDL_BUTTON_LEFT) return;
	if (!isDragging) return;

	if (currentTool == TOOL_LINE)
		canvas.DrawLineDDA((int)startPos.x, (int)startPos.y, (int)currentPos.x, (int)currentPos.y, currentColor);

	else if (currentTool == TOOL_RECT)
		canvas.DrawRect((int)startPos.x, (int)startPos.y,
			(int)(currentPos.x - startPos.x), (int)(currentPos.y - startPos.y),
			currentColor, borderWidth, fillShapes, currentColor);

	else if (currentTool == TOOL_TRI)
	{
		Vector2 p0 = startPos;
		Vector2 p1 = currentPos;
		Vector2 p2 = Vector2(startPos.x, currentPos.y);
		canvas.DrawTriangle(p0, p1, p2, currentColor, fillShapes, currentColor);
	}

	isDragging = false;
}


void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	currentPos = mouse_position;
	if (!isDragging) return;

	if (currentTool == TOOL_PENCIL)
	{
		canvas.DrawLineDDA((int)lastPos.x, (int)lastPos.y, (int)currentPos.x, (int)currentPos.y, currentColor);
		lastPos = currentPos;
	}
	else if (currentTool == TOOL_ERASER)
	{
		canvas.DrawLineDDA((int)lastPos.x, (int)lastPos.y, (int)currentPos.x, (int)currentPos.y, Color::BLACK);
		lastPos = currentPos;
	}
}


void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{
	Shader::ReloadSingleShader(filename);
}