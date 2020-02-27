#include "Tilemap.h"
#include "core/Physics.h"
#include <istream>

Tilemap::Tilemap() : tiles{}
{
}

Tilemap::Tilemap(const char* filepath)
{
	//Loop through the text file and read the numbers
	std::ifstream file(filepath);

	if (!file)
	{
		std::cout << "Tilemap file not found" << std::endl;
	}

	char c = ' ';
	for (int i = 0; i < MAXMAPHEIGHT; i++)
	{
		for (int j = 0; j < MAXMAPWIDTH;)
		{
			file.get(c);
			if (c != ',' && c != ' ' && c != '\n')
			{
				tiles[i][j] = c - 48;
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
	for (Tile& t : tileList)
	{
		Physics::DetectCollision(t, rb);
	}
}
