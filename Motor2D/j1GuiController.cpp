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

	p2List_item<Widget*>* ui_elem_iterator = ui_elems.start;
	while (ui_elem_iterator && ret)
	{
		ret = ui_elem_iterator->data->PreUpdate(d_time);
		ui_elem_iterator = ui_elem_iterator->next;
	}

	return ret;
}

bool j1GuiController::Update(float d_time)
{
	bool ret = true;

	//Draw all ui elements and call their Drag if they have one
	p2List_item<Widget*>* ui_elem_iterator = ui_elems.start;
	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->Drag();
		ui_elem_iterator->data->UpdateAttachedPositions();
		ui_elem_iterator = ui_elem_iterator->next;
	}
	
	return true;
}

bool j1GuiController::CleanUp()
{
	bool ret = true;

	App->tex->UnLoad(atlas);

	p2List_item<Widget*>* ui_iterator;
	ui_iterator = ui_elems.end;

	while (ui_iterator != NULL && ret == true)
	{
		ui_iterator->data->CleanUp();
		ret = DestroyWidget(ui_iterator->data);
		ui_iterator = ui_iterator->prev;
	}
	ui_elems.clear();

	active = false;

	return ret;
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
	case UiElemType::DYNAMIC_LABEL:
		ret = new DynamicLabel(temp_pos, callback);
		break;
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

	if (widget == nullptr)
		ret = false;

	p2List_item<Widget*>* temp = ui_elems.start;
	while (temp && ret)
	{
		if (temp->data == widget)
		{
			if (temp->data->attached_widgets.start)
			{
				p2List_item<Widget*>* attached_elem_iter = temp->data->attached_widgets.start;
				while (attached_elem_iter && temp)
				{
					DestroyWidget(attached_elem_iter->data);
					attached_elem_iter = attached_elem_iter->next;
				}
			}
			delete widget;
			ui_elems.del(temp);
			break;
		}
		temp = temp->next;
	}

	return ret;
}

// const getter for atlas
SDL_Texture* j1GuiController::GetAtlas() const
{
	return atlas;
}

void j1GuiController::Draw()
{
	p2List_item<Widget*>* ui_elem_iterator = ui_elems.start;
	while (ui_elem_iterator)
	{
		ui_elem_iterator->data->Draw();
		ui_elem_iterator = ui_elem_iterator->next;
	}
}

// class Gui ---------------------------------------------------

