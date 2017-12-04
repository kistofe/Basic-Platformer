#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1GuiController.h"
#include "UIWindow.h"

#include "p2Log.h"



UIWindow::UIWindow(iPoint pos, j1Module* callback): Widget(UiElemType::WINDOW, pos, callback)
{
	atlas_section.x = 10;
	atlas_section.y = 423;
	atlas_section.h = 198;
	atlas_section.w = 466;
}

UIWindow::~UIWindow()
{
}

void UIWindow::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &atlas_section);
}

void UIWindow::Drag()
{
	// Check if the window is being clicked or not
	SDL_Point temp_mousepos_sdl;
	App->input->GetMousePosition(temp_mousepos_sdl.x, temp_mousepos_sdl.y);
	temp_mousepos_sdl.x = App->render->ScreenToWorld(temp_mousepos_sdl.x, temp_mousepos_sdl.y).x;
	temp_mousepos_sdl.y = App->render->ScreenToWorld(temp_mousepos_sdl.x, temp_mousepos_sdl.y).y;
	if (!being_clicked && SDL_PointInRect(&temp_mousepos_sdl, &GetArea()) && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		being_clicked = true, LOG("Clicked = TRUE");
	if (being_clicked && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		being_clicked = false, LOG("Clicked = FALSE");
	iPoint temp_mousepos;
	temp_mousepos.create(temp_mousepos_sdl.x, temp_mousepos_sdl.y);

	// Move window if it's clicked and if last mouse position is different from the new one
	if (being_clicked && last_mousepos != temp_mousepos && last_mousepos.x != 0 && last_mousepos.y != 0)
	{
		position += temp_mousepos - last_mousepos;
	}
	last_mousepos = temp_mousepos;

}

SDL_Rect UIWindow::GetArea()
{
	SDL_Rect ret;
	ret.h = atlas_section.h;
	ret.w = atlas_section.w;
	ret.x = position.x;
	ret.y = position.y;
	return ret;
}
