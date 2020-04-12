#ifndef TILE_H
#define TILE_H

#include "components/Components.h"

class Tile : public GameObject, public RigidBodyComponent, public SpriteComponent
{

private:
	int pass; //Used for finding how many times the player has gone through the tile(For Refuels) 

public:

	 enum class TileType
	{
		Normal = 1,
		Refuel,
		Hazard,
		Victory
	} tileType;
	 
	Tile();
	Tile(const Tile &t);
	Tile(const MATH::Vec3& pos, TileType type);
	~Tile();

	// Inherited via GameObject
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	// Inherited via RigidBodyComponent
	virtual void OnCollisionEnter() override;

};

#endif