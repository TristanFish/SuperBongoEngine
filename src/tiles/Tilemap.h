#ifndef TILEMAP_H
#define TILEMAP_H

#define MAXMAPHEIGHT 32
#define MAXMAPWIDTH 32

#include "tiles/Tile.h"

class Tilemap
{
private:

	enum class TileType
	{
		None,
		Normal,
		Hazard,
		Victory
	};

	int tiles[MAXMAPWIDTH][MAXMAPHEIGHT];

public:

};

#endif