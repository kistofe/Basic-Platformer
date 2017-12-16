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

	config = App->LoadUiConfig(ui_elements);
	data = config.child("character_selection");
	textures = data.child("textures");
	labels = data.child("labels");
	buttons = data.child("buttons");
	windows = data.child("windows");
}


j1CharacterSel::~j1CharacterSel()
{
}

bool j1CharacterSel::Start()
{
	AddUiElems();

	background = App->tex->Load(textures.child("background").attribute("source").as_string());
	title = App->tex->Load(textures.child("title").attribute("source").as_string());
	character1_portrait = App->tex->Load(textures.child("char1_portrait").attribute("source").as_string());
	character2_portrait = App->tex->Load(textures.child("char2_portrait").attribute("source").as_string());
	
	return true;
}

bool j1CharacterSel::Update(float d_time)
{
	App->render->Blit(background, textures.child("backgroundpos_x").attribute("value").as_int(), textures.child("backgroundpos_y").attribute("value").as_int());
	App->render->Blit(title, textures.child("titlepos_x").attribute("value").as_int(), textures.child("titlepos_y").attribute("value").as_int());
	
	App->gui->Draw();

	App->render->Blit(character1_portrait, textures.child("portrait1_pos_x").attribute("value").as_int(), textures.child("portrait1_pos_y").attribute("value").as_int());
	App->render->Blit(character2_portrait, textures.child("portrait2_pos_x").attribute("value").as_int(), textures.child("portrait2_pos_y").attribute("value").as_int());
	
	return true;
}

bool j1CharacterSel::CleanUp()
{
	App->tex->UnLoad(background);
	App->tex->UnLoad(title);
	App->tex->UnLoad(character1_portrait);
	App->tex->UnLoad(character2_portrait);

	return true;
}

bool j1CharacterSel::Load(pugi::xml_node& data)
{
	selected_character = data.child("selected_character").attribute("value").as_int();

	return true;
}

bool j1CharacterSel::Save(pugi::xml_node& data) const
{
	data.append_child("selected_character").append_attribute("value") = selected_character;

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
	//READING UI XML FILE TO CREATE THE UI

	pugi::xml_node temp = windows.child("character1_win1");
	//Character 1 Portrait Window
	character1_win = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1_win->SetWindowType(VERTICAL_WINDOW);

	temp = windows.child("character2_win1");
	//Character 2 Portrait Window
	character2_win = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2_win->SetWindowType(VERTICAL_WINDOW);

	temp = windows.child("character1_win2");
	//Character 1 Info Window
	character1_win2 = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1_win2->SetWindowType(HORIZONTAL_WINDOW_S);

	temp = windows.child("character2_win2");
	//Character 2 Info Window
	character2_win2 = (UIWindow*)App->gui->CreateWidget(WINDOW, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2_win2->SetWindowType(HORIZONTAL_WINDOW_S);

	temp = buttons.child("character1");
	//Character 1 Select Button
	character1 = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1->SetButtonType(SELECT_RAMONA);
	character1->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
	{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
	{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });

	temp = buttons.child("character2");
	//Character 2 Select Button
	character2 = (Button*)App->gui->CreateWidget(BUTTON, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2->SetButtonType(SELECT_SCOTT);
	character2->SetSection({ temp.child("idle").attribute("x").as_int() , temp.child("idle").attribute("y").as_int(), temp.child("idle").attribute("w").as_int(), temp.child("idle").attribute("h").as_int() },
	{ temp.child("hovering").attribute("x").as_int(), temp.child("hovering").attribute("y").as_int(), temp.child("hovering").attribute("w").as_int(), temp.child("hovering").attribute("h").as_int() },
	{ temp.child("clicked").attribute("x").as_int(), temp.child("clicked").attribute("y").as_int(), temp.child("clicked").attribute("w").as_int(), temp.child("clicked").attribute("h").as_int() });
	
	temp = labels.child("char1_go");
	//Character 1 Select Label
	character1_go = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1_go->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);
	
	temp = labels.child("char2_go");
	//Character 2 Select Label
	character2_go = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2_go->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	temp = labels.child("char1_name");
	//Character 1 Name Label
	character1_name = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1_name->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->large_size);
	
	temp = labels.child("char2_name");
	//Character 2 Name Label
	character2_name = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2_name->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->large_size);

	temp = labels.child("speed1");
	//Character 1 Speed Value Label
	character1_speed_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1_speed_lab->SetText(temp.child("content").attribute("value").as_string(), {255,255,255}, App->font->medium_size);
	
	temp = labels.child("speed2");
	//Character 2 Speed Value Label
	character2_speed_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2_speed_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255 }, App->font->medium_size);

	temp = labels.child("jump1");
	//Character 1 Jump Value Label
	character1_jump_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character1_jump_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);

	temp = labels.child("jump2");
	//Character 2 Jump Value Label
	character2_jump_lab = (Label*)App->gui->CreateWidget(LABEL, temp.child("position_x").attribute("value").as_int(), temp.child("position_y").attribute("value").as_int(), this);
	character2_jump_lab->SetText(temp.child("content").attribute("value").as_string(), { 255,255,255,255 }, App->font->medium_size);
}
