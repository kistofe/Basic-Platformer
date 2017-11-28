#include "j1App.h"
#include "Button.h"
#include "j1Input.h"

#include "SDL\include\SDL.h"

Button::Button(uint x, uint y)
{}


Button::~Button()
{
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
