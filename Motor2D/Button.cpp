#include "j1App.h"
#include "p2Log.h"
#include "Button.h"
#include "j1Input.h"
#include "Label.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1GuiController.h"
#include "SDL\include\SDL.h"


Button::Button(iPoint pos, j1Module* callback) : Widget(UiElemType::BUTTON, pos, callback)
{
	current_rect = &idle_rect;
}


Button::~Button()
{
}

bool Button::PreUpdate(float d_time)
{
	bool ret = true;

	world_area = { position.x, position.y, current_rect->w, current_rect->h };

	if (MouseOver(world_area) && !hovering)
	{
		hovering = true;
		ChangeVisualState(MOUSE_ENTER);
	}
	if (hovering && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		ChangeVisualState(MOUSE_CLICK);
	}
	if (hovering && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		ChangeVisualState(MOUSE_RELEASE);
		ret = callback->OnEvent(this);
	}
	if (!MouseOver(world_area) && hovering)
	{
		hovering = false;
		ChangeVisualState(MOUSE_LEAVE);
	}
	
	return ret;
}

  
void Button::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, current_rect);
}


bool Button::MouseOver(const SDL_Rect& button)
{
	iPoint mouse_cords;
	App->input->GetMousePosition(mouse_cords.x, mouse_cords.y);
	mouse_cords = App->render->ScreenToWorld(mouse_cords.x, mouse_cords.y);

	return (mouse_cords.x >= button.x && mouse_cords.x <= button.x + button.w) && (mouse_cords.y >= button.y && mouse_cords.y <= button.y + button.h);
}

void Button::SetSection(SDL_Rect idle_sec, SDL_Rect high_sec, SDL_Rect clicked_sec)
{
	idle_rect.x = idle_sec.x;
	idle_rect.y = idle_sec.y;
	idle_rect.w = idle_sec.w;
	idle_rect.h = idle_sec.h;

	highlighted_rect.x = high_sec.x;
	highlighted_rect.y = high_sec.y;
	highlighted_rect.w = high_sec.w;
	highlighted_rect.h = high_sec.h;

	click_rect.x = clicked_sec.x;
	click_rect.y = clicked_sec.y;
	click_rect.w = clicked_sec.w;
	click_rect.h = clicked_sec.h;
}


void Button::SetButtonType(ButtonType type)
{
	button_type = type;
}

void Button::ChangeVisualState(const int event)
{
	switch (event)
	{
	case MOUSE_ENTER:
		current_rect = &highlighted_rect; break;
	case MOUSE_CLICK:
		current_rect = &click_rect; break;
	case MOUSE_RELEASE:
		current_rect = &highlighted_rect; break;
	case MOUSE_LEAVE:
		current_rect = &idle_rect; break;
	}
}
