#include "Tilemap.h"
#include "core/2D/Physics2D.h"
#include <istream>

Tilemap::Tilemap() : tiles{}
{
}

//Enter a filepath to a text file, the text file should be a list of numbers like so
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 1 1 1 1 1 1 1 1 1 1
// 2 2 2 2 2 2 2 2 2 2
// 1 1 1 1 1 1 1 1 1 1
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
//The numbers signify what type of tile to place in that position
//Empty = 0, Normal = 1, Refuel = 2, Hazard = 3, Victory = 4
//At the moment tilemap is locked to only one size of map based on the
//MAXMAPHEIGHT/WIDTH macros in the tilemap header
Tilemap::Tilemap(const char* filepath)
{
	//Open filepath
	std::ifstream file(filepath);

	if (!file)
	{
		std::cout << "Tilemap file not found" << std::endl;
	}

	//loop through textfile
	char c = ' ';
	for (int i = 0; i < MAXMAPHEIGHT; i++)
	{
		for (int j = 0; j < MAXMAPWIDTH;)
		{
			file.get(c);

			//if the current char is a number then add that number to the array of tiles.
			if (c >= '0' && c <= '9')
			{
				tiles[i][j] = c - 48;
				//if the tile type is 0 don't create a tile
				if (tiles[i][j] == 0)
				{
					j++;
					continue;
				}

				CreateTile(j, i, static_cast<Tile::TileType>(tiles[i][j]));
				j++;
			}
		}
	}
	file.close();
}

Tilemap::~Tilemap()
{
	tileList.clear();
}

void Tilemap::CreateTile(int x, int y, Tile::TileType type)
{
	//padding moves the tilemap left/right/up/down
	float paddingx = MAXMAPWIDTH / -2.0f;
	float paddingy = MAXMAPHEIGHT / 2.0f;

	tileList.emplace_back(MATH::Vec3(x + paddingx, -y + paddingy, 0.0f), type);

}

void Tilemap::Update(const float deltaTime)
{
	for (Tile &t : tileList)
	{
		t.Update(deltaTime);
	}
}

void Tilemap::Render() const
{
	for (const Tile &t : tileList)
	{
		t.Render();
	}
}

void Tilemap::CheckCollisions(RigidBodyComponent &rb)
{
	for (Tile &t : tileList)
	{
		Physics2D::DetectCollision(t, rb);
	}
}
