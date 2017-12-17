#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1GuiController.h"
#include "UIWindow.h"

#include "p2Log.h"



UIWindow::UIWindow(iPoint pos, j1Module* callback) : Widget(UiElemType::WINDOW, pos, callback)
{
	//READ FROM XML
	horizontal_window.x = 3;
	horizontal_window.y = 645;
	horizontal_window.h = 192;
	horizontal_window.w = 414;

	vertical_window.x = 647;
	vertical_window.y = 319;
	vertical_window.h = 304;
	vertical_window.w = 272;

	title_window.x = 647;
	title_window.y = 645;
	title_window.h = 60;
	title_window.w = 204;

	horizontal_window_s.x = 647;
	horizontal_window_s.y = 706;
	horizontal_window_s.h = 149;
	horizontal_window_s.w = 302;

}

UIWindow::~UIWindow()
{
}

void UIWindow::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, atlas_section);
}


void UIWindow::SetArea(uint w, uint h)
{
	world_area.w = w;
	world_area.h = h;
}

void UIWindow::SetWindowType(WindowType type)
{
	switch (type)
	{
	case HORIZONTAL_WINDOW:
		atlas_section = &horizontal_window;
		break;
	case VERTICAL_WINDOW:
		atlas_section = &vertical_window;
		break;
	case TITLE_WINDOW:
		atlas_section = &title_window;
		break;
	case HORIZONTAL_WINDOW_S:
		atlas_section = &horizontal_window_s;
		break;
	default:
		LOG("Window Type does not exist");
		break;
	}
	
	SetArea(atlas_section->w, atlas_section->h);
}

