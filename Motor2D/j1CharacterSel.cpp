#include "j1App.h"
#include "j1CharacterSel.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1GuiController.h"
#include "j1SceneSwitch.h"
#include "j1InGameScene.h"
#include "j1EntityManager.h"
#include "j1Map.h"


j1CharacterSel::j1CharacterSel()
{
	name.create("character_selection");
}


j1CharacterSel::~j1CharacterSel()
{
}

bool j1CharacterSel::Start()
{
	AddUiElems();

	background = App->tex->Load("gui/character_select.png");
	title = App->tex->Load("gui/character_select_title.png");
	character1_portrait = App->tex->Load("images/Ramona Character Portrait.png");
	character2_portrait = App->tex->Load("images/Scott Character Portrait.png");
	
	return true;
}

bool j1CharacterSel::Update(float d_time)
{
	App->render->Blit(background, 0, 0);
	App->render->Blit(title, 330, 100);
	
	App->gui->Draw();

	App->render->Blit(character1_portrait, 250, 263);
	App->render->Blit(character2_portrait, 570, 255);
	
	return true;
}

bool j1CharacterSel::CleanUp()
{
	App->tex->UnLoad(background);
	App->tex->UnLoad(title);
	App->tex->UnLoad(character1_portrait);
	App->tex->UnLoad(character2_portrait);

	p2List_item<Widget*>* ui_iterator = App->gui->ui_elems.end;
	while (ui_iterator)
	{
		App->gui->DestroyWidget(ui_iterator->data);
		ui_iterator = ui_iterator->prev;
	}

	return true;
}

bool j1CharacterSel::OnEvent(Button * button)
{
	
	bool ret = true;

	switch (button->button_type)
	{
	case SELECT_RAMONA:
		selected_character = 0;
		App->sceneswitch->SwitchScene(App->ingamescene, this);
		break;
	case SELECT_SCOTT:
		selected_character = 1;
		App->sceneswitch->SwitchScene(App->ingamescene, this);
		break;
	}

	return ret;
}

void j1CharacterSel::AddUiElems()
{
	//Character 1 Portrait Window
	character1_win = (UIWindow*)App->gui->CreateWidget(WINDOW, 240, 220, this);
	character1_win->SetWindowType(VERTICAL_WINDOW);

	//Character 2 Portrait Window
	character2_win = (UIWindow*)App->gui->CreateWidget(WINDOW, 560, 220, this);
	character2_win->SetWindowType(VERTICAL_WINDOW);

	//Character 1 Info Window
	character1_win2 = (UIWindow*)App->gui->CreateWidget(WINDOW, 225, 540, this);
	character1_win2->SetWindowType(HORIZONTAL_WINDOW_S);

	//Character 2 Info Window
	character2_win2 = (UIWindow*)App->gui->CreateWidget(WINDOW, 545, 540, this);
	character2_win2->SetWindowType(HORIZONTAL_WINDOW_S);

	//Character 1 Select Button
	character1 = (Button*)App->gui->CreateWidget(BUTTON, 330, 695, this);
	character1->SetButtonType(SELECT_RAMONA);
	character1->SetSection({ 422, 717, 110, 36 }, { 422, 755, 110, 36 }, { 422, 793, 110, 36 });

	//Character 2 Select Button
	character2 = (Button*)App->gui->CreateWidget(BUTTON, 650, 695, this);
	character2->SetButtonType(SELECT_SCOTT);
	character2->SetSection({ 422, 717, 110, 36 }, { 422, 755, 110, 36 }, { 422, 793, 110, 36 });
	
	//Character 1 Select Label
	character1_go = (Label*)App->gui->CreateWidget(LABEL, 365, 698, this);
	character1_go->SetText("GO!", { 255,255,255,255 }, App->font->medium_size);
	
	//Character 2 Select Label
	character2_go = (Label*)App->gui->CreateWidget(LABEL, 685, 698, this);
	character2_go->SetText("GO!", { 255,255,255,255 }, App->font->medium_size);

	//Character 1 Name Label
	character1_name = (Label*)App->gui->CreateWidget(LABEL, 250, 555, this);
	character1_name->SetText("RAMONA", { 255,255,255,255 }, App->font->large_size);

	//Character 2 Name Label
	character2_name = (Label*)App->gui->CreateWidget(LABEL, 570, 555, this);
	character2_name->SetText("SCOTT", { 255,255,255,255 }, App->font->large_size);

	//Character 1 Speed Value Label
	character1_speed_lab = (Label*)App->gui->CreateWidget(LABEL, 250, 595, this);
	character1_speed_lab->SetText("Speed: ***", {255,255,255}, App->font->medium_size);

	//Character 2 Speed Value Label
	character2_speed_lab = (Label*)App->gui->CreateWidget(LABEL, 570, 595, this);
	character2_speed_lab->SetText("Speed: **", { 255,255,255 }, App->font->medium_size);

	//Character 1 Jump Value Label
	character1_jump_lab = (Label*)App->gui->CreateWidget(LABEL, 250, 625, this);
	character1_jump_lab->SetText("Jump: **", { 255,255,255,255 }, App->font->medium_size);

	//Character 2 Jump Value Label
	character2_jump_lab = (Label*)App->gui->CreateWidget(LABEL, 570, 625, this);
	character2_jump_lab->SetText("Jump: ***", { 255,255,255,255 }, App->font->medium_size);
}
