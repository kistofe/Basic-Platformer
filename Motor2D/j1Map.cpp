#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "Player.h"
#include "j1SceneSwitch.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

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

	//Loop to load all maps
	for (pugi::xml_node map = config.child("map_name"); map && ret; map = map.next_sibling("map_name"))
	{
		p2SString* map_set = new p2SString;

		if (ret == true)
		{
			ret = LoadMapAtrib(map, map_set);
		}
		map_name.add(map_set);
	}

	folder.create(config.child("folder").child_value());
	max_map_x = config.child("max_map_x").attribute("value").as_uint();
	max_map_y = config.child("max_map_y").attribute("value").as_uint();

	return ret;
}

bool j1Map::Update()
{
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
						App->render->Blit(data.tilesets[j]->texture, x*data.tile_width, y*data.tile_height, &data.tilesets[j]->GetTileRect(data.map_layers[i]->layer_gid[data.map_layers[i]->Get(x, y)]), data.map_layers[i]->properties.Get("Parallax speed"));
					}
				}
			}
		}
}

float Properties::Get(const char* value, float default_value) const
{
	p2List_item<Layer_property*>* item = layer_property_list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
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

	// Remove all object groups
	p2List_item<ObjGroup*>* obgroup;
	obgroup = data.objgroup.start;

	while (obgroup != NULL)
	{
		RELEASE(obgroup->data);
		obgroup = obgroup->next;
	}

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
			for (pugi::xml_node obj = obj_layers.child("object"); obj && obj_layers; obj = obj.next_sibling("object"))
			{
				Object* set2 = new Object;
				ret = Load_Object(obj, set2);
				set->object.add(set2);
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

	// Call function to convert objects to colliders
	SetColliders();

	map_loaded = ret;
	
	return ret;
}

iPoint j1Map::MapToWorld(int x, int y) const
{

	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
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
		LoadMapName(map, data.properties);
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

bool j1Map::LoadMapAtrib(pugi::xml_node& config, p2SString* map_set)
{
	bool ret = true;

	map_set->create(config.attribute("name").as_string());

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
	
	pugi::xml_node tile_iterator = layer_node.child("data").child("tile");

	layer->height		= layer_node.attribute("height").as_uint(0);
	layer->width		= layer_node.attribute("width").as_uint(0);
	layer->name.create(layer_node.attribute("name").as_string());
	layer->size			= layer->height * layer->width;
	layer->layer_gid	= new uint[layer->size];
	LoadLayerProperties(layer_node, layer->properties);

	memset(layer->layer_gid, 0, (sizeof(uint)*layer->size));

	
	for (int i = 0; i < layer->size; i++)
	{
		layer->layer_gid[i] = tile_iterator.attribute("gid").as_uint();
		tile_iterator = tile_iterator.next_sibling("tile");
	}

	if (layer->properties.Get("Navigation") == 1)
		App->pathfinding->SetMap(layer->width, layer->height, layer->layer_gid);
	
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

bool j1Map::LoadLayerProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Layer_property* p = new Properties::Layer_property();
			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_float();

			properties.layer_property_list.add(p);
		}
	}

	return ret;
}

bool j1Map::LoadMapName(pugi::xml_node& node, Properties & properties)
{
	bool ret = true;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Map_name* p2 = new Properties::Map_name();
			p2->name = prop.attribute("name").as_string();
			p2->value = prop.attribute("value").as_string();

			properties.map_name_list.add(p2);
		}
	}

	return ret;
}

bool j1Map::LoadObjectProperties(pugi::xml_node & node, Properties & properties)
{
	bool ret = true;
	
	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Object_property* p2 = new Properties::Object_property();
			p2SString temp_name = prop.attribute("name").as_string(); //Temporary string to store the name of the current property and compare

			if (temp_name == "collider_offset_x")
				p2->collider_offset.x = prop.attribute("value").as_int();
			if (temp_name == "collider_offset_y")
				p2->collider_offset.y = prop.attribute("value").as_int();
			if (temp_name == "moving_speed")
				p2->moving_speed = prop.attribute("value").as_float();
			if (temp_name == "jumping_speed")
				p2->jumping_speed = prop.attribute("value").as_float();
			if (temp_name == "death_sfx_source")
				p2->death_sfx_source = prop.attribute("value").as_string();
			if (temp_name == "jumping_sfx_source")
				p2->jump_sfx_source = prop.attribute("value").as_string();
			if (temp_name == "landing_sfx_source")
				p2->land_sfx_source = prop.attribute("value").as_string();
			
			properties.obj_property_list.add(p2);
		}
	}

	return ret;
}
bool j1Map::CreateWalkabilityMap(int & width, int & height, uchar ** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.map_layers.start;

	for (item = data.map_layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->GetID(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}

TileSet * j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}


bool j1Map::MapSwitch(char* new_map)
{
	bool ret = true;

	//calls cleanUp to unload current map
	ret = CleanUp();

	if (ret == true)//loads new map
	{
		Load(new_map);
	}

	return ret;
}

// Any objects that should become colliders do so here
bool j1Map::SetColliders()
{
	p2List_item<ObjGroup*>* current_objgroup = data.objgroup.start;

	while (current_objgroup)
	{
		p2List_item<Object*>* current_object = current_objgroup->data->object.start;
		while (current_object)
		{
			SDL_Rect collider_tocreate;
			if (current_object->data->name == "wall")
			{
				collider_tocreate.x = current_object->data->x;
				collider_tocreate.y = current_object->data->y;
				collider_tocreate.w = current_object->data->width;
				collider_tocreate.h = current_object->data->height;
				App->collision->AddCollider(collider_tocreate, COLLIDER_WALL);
			}

			if (current_object->data->name == "LevelEnd")
			{
				collider_tocreate.x = current_object->data->x;
				collider_tocreate.y = current_object->data->y;
				collider_tocreate.w = current_object->data->width;
				collider_tocreate.h = current_object->data->height;
				App->collision->AddCollider(collider_tocreate, COLLIDER_ENDOFLEVEL);
			}

			if (current_object->data->name == "death")
			{
				collider_tocreate.x = current_object->data->x;
				collider_tocreate.y = current_object->data->y;
				collider_tocreate.w = current_object->data->width;
				collider_tocreate.h = current_object->data->height;
				App->collision->AddCollider(collider_tocreate, COLLIDER_DEATH);
			}
			current_object = current_object->next;
		}
		current_objgroup = current_objgroup->next;
	}


	return true;
}

bool j1Map::SetEntities()
{
	//Read from object layer in tiled where enemies are
	p2List_item<ObjGroup*>* current_objgroup = data.objgroup.start;
	while (current_objgroup)
	{
		p2List_item<Object*>* current_entity = current_objgroup->data->object.start;
		while (current_entity)
		{
			if (current_entity->data->name == "Player")
				App->entities->CreateEntity(Entity::EntityType::PLAYER,  current_entity->data->x, current_entity->data->y );
					
			if (current_entity->data->name == "Flying_Enemy")
				App->entities->CreateEntity(Entity::EntityType::FLYING_ENEMY,  current_entity->data->x, current_entity->data->y );

			if (current_entity->data->name == "Ground_Enemy")
				App->entities->CreateEntity(Entity::EntityType::GROUND_ENEMY,  current_entity->data->x, current_entity->data->y );

			current_entity = current_entity->next;
		}
		current_objgroup = current_objgroup->next;
	}
	
	return true;
}