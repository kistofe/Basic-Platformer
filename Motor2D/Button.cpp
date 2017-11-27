#include "j1App.h"
#include "Button.h"
#include "j1Input.h"

#include "SDL\include\SDL.h"

Button::Button(uint x, uint y) : Widgets(Widgets::UiElemType::BUTTON)
{}


Button::~Button()
{
}

bool Button::Start()
{
	return true;
}

bool Button::PreUpdate()
{
	return true;
}

bool Button::Update()
{
	return true;
}

bool Button::PostUpdate()
{
	return true;
}

bool Button::CleanUp()
{
	return true;
}

void Button::Draw()
{
}

void Button::OnClick()
{
}

bool Button::CheckClick(const SDL_Rect& button)
{
	bool ret = false;
	bool is_over = MouseOver(button);

	if (is_over && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT == KEY_DOWN))
		ret = true;

	return ret;
}

bool Button::MouseOver(const SDL_Rect& button)
{
	int x, y;
	App->input->GetMousePosition(x, y);

	return (x >= button.x && x <= button.x + button.w) && (y >= button.y && y <= button.y + button.h);
}
