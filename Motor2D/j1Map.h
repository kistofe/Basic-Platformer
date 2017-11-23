#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

#include "SDL/include/SDL.h"

// ----------------------------------------------------
struct Properties
{
	struct Layer_property
	{
		p2SString name;
		float value;
	};

	struct Map_name
	{
		p2SString name;
		p2SString value;
	};

	~Properties()
	{
		p2List_item<Layer_property*>* item;
		item = layer_property_list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		layer_property_list.clear();


		p2List_item<Map_name*>* item2;
		item2 = map_name_list.start;

		while (item2 != NULL)
		{
			RELEASE(item2->data);
			item2 = item2->next;
		}

		map_name_list.clear();


	}

	float Get(const char* name, float default_value = 0) const; 

	p2List<Layer_property*>	layer_property_list;
	p2List<Map_name*> map_name_list;
};
// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	uint		width;
	uint		height;
	uint		size;
	uint*		layer_gid;
	Properties	properties;
	MapLayer() : layer_gid(nullptr) {}
	~MapLayer() { RELEASE(layer_gid); };

	inline uint Get(uint x, uint y)
	{
		return x + y*width;
	}

	inline uint GetID(uint x, uint y) const
	{
		return layer_gid[(y*width) + x];
	}
};

struct Object
{
	p2SString	name;
	uint		object_id;
	uint		width;
	uint		height;
	uint		size;
	uint		x;
	uint		y;
	Properties properties;

};

struct ObjGroup
{
	p2SString			group_name;
	Properties			properties;
	p2List<Object*>		object;
	
	~ObjGroup()
	{
		p2List_item<Object*>* item = object.start;

		while (item)
		{
			RELEASE(item->data);
			item = item->next;
		}
		object.clear();
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;
	
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};
// ----------------------------------------------------
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	Properties					properties;
	p2SString					name;
	int							width;
	int							height;
	int							tile_width;
	int							tile_height;
	SDL_Color					background_color;
	MapTypes					type;
	p2List<TileSet*>			tilesets;
	p2List<MapLayer*>			map_layers;
	p2List<ObjGroup*>			objgroup;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Update();

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	
	// Change from map coordinates (tiles) to world coordinates (pixels)
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	// Method to darken screen while map switching takes place

	bool MapSwitch(char*);

	bool SetColliders();

	bool SetEntities();

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	p2SString LoadMapAtrib(pugi::xml_node&, p2SString*);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& layer_node, MapLayer* layer);
	bool Load_ObjectGroup(pugi::xml_node& obj_node, ObjGroup* obj);
	bool Load_Object(pugi::xml_node& obj_node, Object* obj);
	bool LoadLayerProperties(pugi::xml_node& node, Properties& properties);
	bool LoadMapName(pugi::xml_node&, Properties& properties);
	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	p2List<p2SString*> map_name;
	uint max_map_x;

private:

	// Variables for map loading
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	
};

#endif // __j1MAP_H__