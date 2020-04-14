#include "Tile.h"
#include "custom/Player.h"

Tile::Tile():  tileType(TileType::Normal)
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

	RigidBodyComponent::setMoveable(false);
 

	//Setup different sprites for each tiletype

	//Normal Tiles
	if (tileType == TileType::Normal) 
	{
		collider.isTrigger = false;
		SpriteComponent::setTexture("src/Textures/Tiles/tile003.jpg");
	} 
	else if (tileType == TileType::Normal_2)
	{
		collider.isTrigger = false;
		SpriteComponent::setTexture("src/Textures/Tiles/tile002.jpg");
	}
	else if (tileType == TileType::UnderGround)
	{
		collider.isTrigger = false;
		SpriteComponent::setTexture("src/Textures/Tiles/tile006.jpg");
	}

	//Hazard Tiles
	else if (tileType == TileType::Hazard)
	{
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/Tiles/tile143.png");
	}

	//Refuel tiles
	else if (tileType == TileType::Refuel)
	{
		pass = 0;
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/Tiles/tile013.jpg");
	}
	else if (tileType == TileType::Refuel_2)
	{
		pass = 0;
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/Tiles/tile013_2.jpg");
	}

	//Chains
	else if (tileType == TileType::Chain)
	{
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/Tiles/tile191.png");
	}
	else if (tileType == TileType::Chain_Bot)
	{
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/Tiles/tile192.png");
	}

	else if (tileType == TileType::Victory)
	{
		collider.isTrigger = true;
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

void Tile::OnCollisionEnter(RigidBodyComponent& otherBody)
{
	if (tileType == TileType::Normal || tileType == TileType::Normal_2)
	{

	}
	else if (tileType == TileType::Refuel || tileType == TileType::Refuel_2 && pass == 0)
	{
		SpriteComponent::setTexture("src/Textures/Tiles/Blank.png");
		pass = 1;
	} else if (tileType == TileType::Hazard)
	{

	} else if (tileType == TileType::Victory)
	{

	}
}
