#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1GuiController.h"

j1GuiController::j1GuiController() : j1Module()
{
	name.create("gui");
}

// Destructor
j1GuiController::~j1GuiController()
{}

// Called before render is available
bool j1GuiController::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1GuiController::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	p2List_item <Widgets* >* ui_elem_iterator = ui_elems.start;

	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->Start();
		ui_elem_iterator = ui_elem_iterator->next;
	}

	return true;
}

// Update all guis
bool j1GuiController::PreUpdate()
{
	p2List_item <Widgets* >* ui_elem_iterator = ui_elems.start;

	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->PreUpdate();
		ui_elem_iterator = ui_elem_iterator->next;
	}

	return true;
}

// Called after all Updates
bool j1GuiController::PostUpdate()
{
	p2List_item <Widgets* >* ui_elem_iterator = ui_elems.start;

	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->PostUpdate();
		ui_elem_iterator = ui_elem_iterator->next;
	}

	return true;
}

// Called before quitting
bool j1GuiController::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<Widgets*>* ui_elem_iterator = ui_elems.end;
	
	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->CleanUp();
		ui_elem_iterator = ui_elem_iterator->prev;
	}


	return true;
}

Widgets* j1GuiController::CreateWidget(Widgets::UiElemType type, uint x, uint y)
{
	Widgets* ret = nullptr;

	switch (type)
	{
	case Widgets::UiElemType::BUTTON:			ret = new Button(x, y);
		break;
	case Widgets::UiElemType::LABEL:			ret = new Label(x, y);
		break;
	case Widgets::UiElemType::DYNAMIC_LABEL:	ret = new DynamicLabel(x, y);
	}


	if (ret != nullptr)
		ui_elems.add(ret);
	
	return ret;
}

bool j1GuiController::DestroyWidget(Widgets* widget)
{
	bool ret = true;

	if (widget != nullptr)
		delete widget;

	else
		ret = false;

	return ret;
}

// const getter for atlas
const SDL_Texture* j1GuiController::GetAtlas() const
{
	return atlas;
}


// class Gui ---------------------------------------------------

