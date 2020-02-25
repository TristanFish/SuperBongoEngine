#ifndef TILE_H
#define TILE_H

#include "components/Components.h"

class Tile : public GameObject, public RigidBodyComponent, public SpriteComponent
{
private:



public:

	Tile();
	~Tile();


	// Inherited via GameObject
	virtual void Update(const float deltaTime) override = 0;
	virtual void Render() const override = 0;
	virtual void HandleEvents(const SDL_Event& event) override = 0;

	// Inherited via RigidBodyComponent
	virtual void OnCollisionEnter() override = 0;
};

#endif