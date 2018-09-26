#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum renderorder_ENUM { RIGHT_DOWN, RIGHT_UP, LEFT_DOWN, LEFT_UP };
enum orientation_ENUM { ORTHOGONAL, ISOMETRIC };


// TODO 2: Create a struct to hold information for a TileSet
struct TileSetInfo {
	char* name;
	unsigned int firstguid;
	unsigned int tilewidth;
	unsigned int tileheight;
	unsigned int spacing;
	unsigned int margin;
};
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------


// TODO 1: Create a struct needed to hold the information to Map node

struct MapInfo {

	orientation_ENUM orientation;
	renderorder_ENUM renderorder;
	unsigned int width;
	unsigned int height;
	unsigned int tilewidth;
	unsigned int tileheight;

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

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	

private:
	bool LoadTileMap(MapInfo &mapinfo, TileSetInfo &tilesetinfo);
	pugi::xml_document MapInfos;
public:

	// TODO 1: Add your struct for map info as public for now

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

};

#endif // __j1MAP_H__