#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

bool j1Map::Update(float dt)
{
	// Replace condition of if with what triggers scene switching (getting to the end of the level)
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		switching = true;
		FadeToBlack(0.3f);
	}
		
	if (switching)
	{
		if (current_step == fade_step::none)
			return true;

		Uint32 now = SDL_GetTicks() - start_time;
		float normalized = MIN(1.0f, (float)now / (float)total_time);

		switch (current_step)
		{
		case fade_step::fade_to_black:
		{
			if (now >= total_time)
			{
				MapSwitch("test2.tmx");
				total_time += total_time;
				start_time = SDL_GetTicks();
				fading = false;
				current_step = fade_step::fade_from_black;
			}
		} break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;

			if (now >= total_time)
			{
				current_step = fade_step::none;
			}


		} break;
		}

		// Finally render the black square with alpha on the screen
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
		SDL_RenderFillRect(App->render->renderer, &screen);
	}

	return true;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;


	//loop through all layers and all tiles 
	for (int i = 0; i < data.map_layers.count(); i++)
	{
		for (int j = 0; j < data.tilesets.count(); j++)
		{
			for (int y = 0; y < data.height; y++)
			{
				for (int x = 0; x < data.width; x++)
				{
					App->render->Blit(data.tilesets[j]->texture, x*data.tile_width, y*data.tile_height, &data.tilesets[j]->GetTileRect(data.map_layers[i]->layer_gid[data.map_layers[i]->Get(x, y)]));
				}
			}
		}
	}

}


SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	
	// Remove all layers
	p2List_item<MapLayer*>* layer;
	layer = data.map_layers.start;

	while (layer != NULL)
	{
		RELEASE(layer->data);
		layer = layer->next;
	}
	
	data.map_layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layers;
	for (layers = map_file.child("map").child("layer"); layers && ret; layers = layers.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layers, set);
		}

		data.map_layers.add(set);
	}

	// Load Object info --------------------------------------------
	pugi::xml_node obj_layers;
	for (obj_layers = map_file.child("map").child("objectgroup"); obj_layers && ret; obj_layers = obj_layers.next_sibling("objectgroup"))
	{
		ObjGroup* set = new ObjGroup;
		if (ret == true)
		{
			ret = Load_ObjectGroup(obj_layers, set);
			Object* set2 = new Object;
			for (pugi::xml_node obj = obj_layers.child("object"); obj && obj_layers; obj = obj.next_sibling("object"))
			{
				ret = Load_Object(obj, set2);
				data.object.add(set2);
			}
		}
		data.objgroup.add(set);
			
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

				
		p2List_item<MapLayer*>* item_layer = data.map_layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

bool j1Map::FadeToBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		fading = true;
		ret = true;
	}

	return ret;
}



// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& layer_node, MapLayer* layer)
{
	bool ret = true;
	
	pugi::xml_node sib_iterator = layer_node.child("data").child("tile");

	layer->height		= layer_node.attribute("height").as_uint(0);
	layer->width		= layer_node.attribute("width").as_uint(0);
	layer->name.create(layer_node.attribute("name").as_string());
	layer->size			= layer->height * layer->width;
	layer->layer_gid	= new uint[layer->size];
	
	memset(layer->layer_gid, 0, (sizeof(uint)*layer->size));

	
	for (int i = 0; i < layer->size; i++)
	{
		layer->layer_gid[i] = sib_iterator.attribute("gid").as_uint();
		sib_iterator = sib_iterator.next_sibling("tile");
	}
	
	return ret;
}

bool j1Map::Load_ObjectGroup(pugi::xml_node& objgroup_node, ObjGroup* objgroup)
{
	bool ret = true;
	Object obj;
	objgroup->group_name.create(objgroup_node.attribute("name").as_string());

	
	return ret;
}

bool j1Map::Load_Object(pugi::xml_node& obj_node, Object* obj)
{
	bool ret = true;
	
	obj->height			= obj_node.attribute("height").as_uint(0);
	obj->width			= obj_node.attribute("width").as_uint(0);
	obj->size			= obj->width * obj->height;
	obj->name.create(obj_node.attribute("name").as_string());
	obj->x				= obj_node.attribute("x").as_uint(0);
	obj->y				= obj_node.attribute("y").as_uint(0);
	obj->object_id		= obj_node.attribute("id").as_uint(0);


	return ret;
}

bool j1Map::MapSwitch(char * new_map)
{
	bool ret = true;

	UnloadCurrentMap();

	Load(new_map);

	return ret;
}

bool j1Map::UnloadCurrentMap()
{
	LOG("Unloading current map");
	bool ret = true;

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();


	// Remove all layers
	p2List_item<MapLayer*>* layer;
	layer = data.map_layers.start;

	while (layer != NULL)
	{
		RELEASE(layer->data);
		layer = layer->next;
	}

	data.map_layers.clear();

	// Remove all obects
	p2List_item<ObjGroup*>* objgroup;
	objgroup = data.objgroup.start;

	while (objgroup != NULL)
	{
		RELEASE(objgroup->data);
		objgroup = objgroup->next;
	}

	data.objgroup.clear();

	// Clean up the pugui tree
	map_file.reset();


	return ret;
}




