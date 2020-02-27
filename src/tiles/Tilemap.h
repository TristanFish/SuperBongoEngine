#ifndef TILEMAP_H
#define TILEMAP_H

#include "tiles/Tile.h"

#define MAXMAPHEIGHT 10
#define MAXMAPWIDTH 10

class Tilemap
{
private:

	int tiles[MAXMAPWIDTH][MAXMAPHEIGHT];
	std::vector<Tile> tileList;

public:

	Tilemap();
	Tilemap(const char* filepath);
	~Tilemap();

	void Update(const float deltaTime);
	void Render() const;
	void CheckCollisions(RigidBodyComponent &rb);

};



#endif