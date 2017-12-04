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

bool j1GuiController::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}
bool j1GuiController::PreUpdate(float d_time)
{
	bool ret = true;

	//Draw all ui elements 
	p2List_item<Widget*>* ui_elem_iterator = ui_elems.start;
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
	p2List_item<Widget*>* ui_elem_iterator = ui_elems.start;
	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->Draw();
		ui_elem_iterator = ui_elem_iterator->next;
	}
	
	return true;
}

Widget* j1GuiController::CreateWidget(UiElemType type, uint x, uint y, j1Module* callback)
{
	Widget* ret = nullptr;

	//Temporary position variable that is passed to the constructors of the elements
	iPoint temp_pos;
	temp_pos.create(x, y);

	switch (type)
	{
	case UiElemType::BUTTON:			
		ret = new Button(temp_pos, callback);
		break;
	case UiElemType::LABEL:	
		ret = new Label(temp_pos, callback);
		break;
	//case UiElemType::DYNAMIC_LABEL:	ret = new DynamicLabel(x, y);
	case UiElemType::WINDOW:
		ret = new UIWindow(temp_pos, callback);
		break;
	}


	if (ret != nullptr)
		ui_elems.add(ret);
	
	return ret;
}

bool j1GuiController::DestroyWidget(Widget* widget)
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

