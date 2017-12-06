#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "Player.h"
#include "j1Scene.h"
#include "j1SceneSwitch.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1GuiController.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene"); 
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;


	return ret;
}



