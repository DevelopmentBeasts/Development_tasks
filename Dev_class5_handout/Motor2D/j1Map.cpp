#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
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

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	MapLayer *layer = mapinfo.Layers.start->data;
	TileSet*tileset = mapinfo.tilesets.start->data;
	
	

	SDL_Rect e = tileset->GetTileRect(20);
	SDL_Rect ATile;
	bool ret = true;

	for (int y = 0; y < layer->Height; ++y) {
		for (int x = 0; x < layer->Witdh; ++x) {

			uint gid = layer->CoreData[(y*layer->Witdh)+x];
			SDL_Rect rect = tileset->GetTileRect(gid);
			App->render->Blit(tileset->texture, x*tileset->tile_width, y*tileset->tile_height, &rect);
		}
	}
	



	/*for (int i = 0; ret;++i) {

		ATile = tileset->GetTileRect(i);
		if ( == nullptr) {
			ret = false;
		}
	}*/







		// TODO 9: Complete the draw function

}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * mapinfo.tile_width;
	ret.y = y * mapinfo.tile_height;

	return ret;
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
	item = mapinfo.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapinfo.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers

	p2List_item<MapLayer*>* item_Layer;
	item_Layer = mapinfo.Layers.start;

	while (item != NULL)
	{
		RELEASE(item_Layer->data);
		item_Layer = item_Layer->next;
	}
	mapinfo.tilesets.clear();


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

		mapinfo.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------

	pugi::xml_node LayerNode;



	for (LayerNode = map_file.child("layer"); LayerNode && ret; LayerNode=LayerNode.next_sibling("layer")) {

	MapLayer*layer_ = new MapLayer();

	LoadLayer(LayerNode, layer_);
		
	}


	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", mapinfo.width, mapinfo.height);
		LOG("tile_width: %d tile_height: %d", mapinfo.tile_width, mapinfo.tile_height);

		p2List_item<TileSet*>* item = mapinfo.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = mapinfo.Layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->Witdh, l->Height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

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
		mapinfo.width = map.attribute("width").as_int();
		mapinfo.height = map.attribute("height").as_int();
		mapinfo.tile_width = map.attribute("tilewidth").as_int();
		mapinfo.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		mapinfo.background_color.r = 0;
		mapinfo.background_color.g = 0;
		mapinfo.background_color.b = 0;
		mapinfo.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) mapinfo.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) mapinfo.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) mapinfo.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			mapinfo.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			mapinfo.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			mapinfo.type = MAPTYPE_STAGGERED;
		}
		else
		{
			mapinfo.type = MAPTYPE_UNKNOWN;
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

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	
	layer->Height = node.attribute("height").as_uint();
	layer->Witdh = node.attribute("width").as_uint();
	layer->name = node.attribute("name").as_string();

	layer->CoreData = new unsigned int[layer->Witdh*layer->Height];
	
	memset(layer->CoreData,0u,(layer->Witdh*layer->Height));
	
	for (uint i = 0; i < (layer->Witdh*layer->Height);++i) {

		layer->CoreData[i] = node.attribute("tile").as_uint();
		node=node.next_sibling("tile");
	}

	

	return true;
}