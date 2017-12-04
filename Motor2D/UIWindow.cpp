#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1GuiController.h"
#include "UIWindow.h"



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