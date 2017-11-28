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
	return true;
}

// Update all guis
bool j1GuiController::PreUpdate()
{
	return true;
}

bool j1GuiController::Update()
{
	return true;
}

// Called after all Updates
bool j1GuiController::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1GuiController::CleanUp()
{
	return true;
}

Widgets* j1GuiController::CreateWidget(UiElemType type, uint x, uint y, Widgets* callback)
{
	Widgets* ret = nullptr;

	switch (type)
	{
	case UiElemType::BUTTON:			ret = new Button(x, y);
		break;
	case UiElemType::LABEL:			ret = new Label(x, y);
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
const SDL_Texture* j1GuiController::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

