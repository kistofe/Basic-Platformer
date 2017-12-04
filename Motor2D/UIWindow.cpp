#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1GuiController.h"
#include "UIWindow.h"



UIWindow::UIWindow(iPoint pos, j1Module* callback): Widget(UiElemType::WINDOW, pos, callback)
{
	atlas_section.x = 32;
	atlas_section.y = 543;
	atlas_section.h = 421;
	atlas_section.w = 454;
}

UIWindow::~UIWindow()
{
}

void UIWindow::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &atlas_section);
}