#include "Tile.h"

Tile::Tile()
{
	RigidBodyComponent::Init(this);
	SpriteComponent::Init(this);
}

Tile::~Tile()
{
}

void Tile::Update(const float deltaTime)
{
}

void Tile::Render() const
{
}

void Tile::HandleEvents(const SDL_Event& event)
{
}

void Tile::OnCollisionEnter()
{
}
