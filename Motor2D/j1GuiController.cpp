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

	atlas_file_name = conf.child("atlas").attribute("file").as_string();
	atlas = App->tex->Load(atlas_file_name.GetString());

	return ret;
}

bool j1GuiController::PreUpdate(float d_time)
{
	bool ret = true;

	//Draw all ui elements 
	p2List_item<Widgets*>* ui_elem_iterator = ui_elems.start;
	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->PreUpdate(d_time);
		ui_elem_iterator = ui_elem_iterator->next;
	}

	return ret;
}

bool j1GuiController::Update(float d_time)
{
	bool ret = true;

	//Draw all ui elements 
	p2List_item<Widgets*>* ui_elem_iterator = ui_elems.start;
	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->Draw();
		ui_elem_iterator = ui_elem_iterator->next;
	}
	
	return true;
}

Widgets* j1GuiController::CreateWidget(Widgets::UiElemType type, uint x, uint y, j1Module* callback)
{
	Widgets* ret = nullptr;

	switch (type)
	{
	case Widgets::UiElemType::BUTTON:			
		ret = new Button(x, y, callback);
		break;
	case Widgets::UiElemType::LABEL:	
		ret = new Label(x, y, callback);
		break;
	//case UiElemType::DYNAMIC_LABEL:	ret = new DynamicLabel(x, y);
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
SDL_Texture* j1GuiController::GetAtlas() const
{
	return atlas;
}


// class Gui ---------------------------------------------------

