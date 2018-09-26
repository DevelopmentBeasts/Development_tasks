#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum orientation_enum{ORTHOGONAL,ISOMETRIC};
enum renderorder_enum{RIGHT_DOWN,RIGHT_UP,LEFT_UP,LEFT_DOWN};
// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

struct TileSetInfo {
	char*name;
	unsigned int firstguid;
	unsigned int tilewidth;
	unsigned int tileheight;
	unsigned int spacing;
	unsigned int margin;
};
struct MapStuff {
	orientation_enum orientation;
	renderorder_enum renderorder;
	unsigned int width;
	unsigned int height;
	unsigned int titlewidth;
	unsigned int titleheight;
};
// ----------------------------------------------------


// TODO 1: Create a struct needed to hold the information to Map node


// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);



public:

	// TODO 1: Add your struct for map info as public for now

	
public:
	bool LoadMap(pugi::xml_node nodemap, MapStuff &stuff);
	bool LoadTileSet();
	MapStuff Map;
private:

	MapStuff mapstuffvar;
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__