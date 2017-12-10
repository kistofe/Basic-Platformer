#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1GuiController.h"
#include "UIWindow.h"

#include "p2Log.h"



UIWindow::UIWindow(iPoint pos, j1Module* callback): Widget(UiElemType::WINDOW, pos, callback)
{
	horizontal_window.x = 10;
	horizontal_window.y = 429;
	horizontal_window.h = 192;
	horizontal_window.w = 414;

	vertical_window.x = 10;
	vertical_window.y = 639;
	vertical_window.h = 304;
	vertical_window.w = 272;

}

UIWindow::~UIWindow()
{
}

void UIWindow::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, atlas_section);
}


void UIWindow::SetArea()
{
	world_area.h = atlas_section->h;
	world_area.w = atlas_section->w;
	world_area.x = position.x;
	world_area.y = position.y;
}

void UIWindow::SetWindowType(uint window_type)
{
	if (window_type == 1)
		atlas_section = &vertical_window;
	else if (window_type == 2)
		atlas_section = &horizontal_window;
	else
		LOG("Window Type does not exist");
}
