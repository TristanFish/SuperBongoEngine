#include "Tile.h"

Tile::Tile(): tileType(TileType::Normal)
{
	RigidBodyComponent::Init(this);
	SpriteComponent::Init(this);
}

Tile::Tile(const Tile& t) : Tile(t.transform.pos, t.tileType)
{
}

Tile::Tile(const MATH::Vec3& pos, TileType type): tileType(type)
{
	name = "Tile";
	transform.setPos(pos);

	RigidBodyComponent::Init(this);
	SpriteComponent::Init(this);

	//Setup different sprites for each tiletype
	if (tileType == TileType::Normal) 
	{
		SpriteComponent::setTexture("src/placeholderSquare.png");
	} 
	else if (tileType == TileType::Hazard)
	{
		SpriteComponent::setTexture("src/test.jpg");
	}
	else if (tileType == TileType::Refuel)
	{
		SpriteComponent::setTexture("ReSize.png");
	}
	else if (tileType == TileType::Victory)
	{

	}
	SpriteComponent::setShaders("src/graphics/ShaderVert.glsl", "src/graphics/ShaderText.glsl");
}

Tile::~Tile()
{
}

void Tile::Update(const float deltaTime)
{
	transform.Update(deltaTime);
	RigidBodyComponent::Update(deltaTime);
	SpriteComponent::Update(deltaTime);
}

void Tile::Render() const
{
	SpriteComponent::Render();
}

void Tile::HandleEvents(const SDL_Event& event)
{
}

void Tile::OnCollisionEnter()
{
	if (tileType == TileType::Normal)
	{
		std::cout << "Normal tile touched" << std::endl;
	} else if (tileType == TileType::Refuel)
	{
		std::cout << "Refuel tile touched" << std::endl;
	} else if (tileType == TileType::Hazard)
	{
		std::cout << "Hazard tile touched" << std::endl;
	} else if (tileType == TileType::Victory)
	{
		std::cout << "Victory tile touched" << std::endl;
	}
}
