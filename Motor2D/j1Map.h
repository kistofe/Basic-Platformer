#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	uint		width;
	uint		height;
	uint		size;
	uint*		layer_gid;
	~MapLayer() { delete layer_gid; };


	inline uint Get(uint x, uint y)
	{
		return x + y*width;
	}
};

struct ObjGroup
{
	p2SString	group_name;

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
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	map_layers;
	p2List<ObjGroup*>	objgroup;
	p2List<Object*>		object;
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

	bool Update(float dt);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// Change from map coordinates (tiles) to world coordinates (pixels)
	iPoint MapToWorld(int x, int y) const;

	// Method to darken screen while map switching takes place
	bool FadeToBlack(float time = 1.0f);


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& layer_node, MapLayer* layer);
	bool Load_ObjectGroup(pugi::xml_node& obj_node, ObjGroup* obj);
	bool Load_Object(pugi::xml_node& obj_node, Object* obj);

	bool MapSwitch(char* new_map);

	bool UnloadCurrentMap();

public:

	MapData data;

	// Variables for map switching
	bool				fading			= false;

private:

	// Variables for map loading
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

	// Variables for map switching
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	}					current_step	= fade_step::none;

	Uint32				start_time		= 0;
	Uint32				total_time		= 0;
	SDL_Rect			screen;
	bool				switching		= false;
};

#endif // __j1MAP_H__