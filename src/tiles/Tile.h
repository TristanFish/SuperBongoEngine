#ifndef TILE_H
#define TILE_H

#include "components/Components.h"

class Tile : public GameObject, public RigidBodyComponent, public SpriteComponent
{
private:



public:

	enum class TileType
	{
		Normal,
		Refuel,
		Hazard,
		Victory
	} tileType;

	Tile();
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