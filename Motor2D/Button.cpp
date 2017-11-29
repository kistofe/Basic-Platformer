#include "j1App.h"
#include "p2Log.h"
#include "Button.h"
#include "j1Input.h"
#include "Label.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1EntityManager.h" //debug
#include "j1GuiController.h"

#include "SDL\include\SDL.h"


Button::Button(uint x, uint y, j1Module* callback) : Widgets(Widgets::UiElemType::BUTTON)
{
	this->callback = callback;
	position.x = x;
	position.y = y;
	current_rect = &idle_rect;
}


Button::~Button()
{
}

bool Button::PreUpdate(float d_time)
{
	area = { position.x, position.y, current_rect->w, current_rect->h };

	if (MouseOver(area) && !hovering)
	{
		hovering = true;
		LOG("Is over");
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			LOG("Clicked");
		}
			
	}
	if (!MouseOver(area) && hovering)
	{
		hovering = false;
		LOG("Not over");
	}
	
	return true;
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

void Button::SetSection(SDL_Rect button_rec)
{
	idle_rect.x = button_rec.x;
	idle_rect.y = button_rec.y;
	idle_rect.w = button_rec.w;
	idle_rect.h = button_rec.h;
}

void Button::CreateButtonLabel(Label* button_label, const char* content, SDL_Color color)
{
	text = button_label;
	text->SetText(content, color);
}
