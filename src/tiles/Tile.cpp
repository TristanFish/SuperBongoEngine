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
	if (tileType == TileType::Normal) 
	{
		collider.isTrigger = false;
		SpriteComponent::setTexture("src/Textures/texture_09.jpg");
	} 
	else if (tileType == TileType::Hazard)
	{
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/Hazard.png");
	}
	else if (tileType == TileType::Refuel)
	{
		pass = 0;
		collider.isTrigger = true;
		SpriteComponent::setTexture("src/Textures/texture_02.jpg");
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

void Tile::OnCollisionEnter()
{
	if (tileType == TileType::Normal)
	{
		std::cout << "Normal tile touched" << std::endl;
	}
	else if (tileType == TileType::Refuel && pass == 0)
	{
		Player::setJetPower(Player::getJetPower() + 2.0f);
		SpriteComponent::setTexture("src/Textures/texture_08.jpg");
		std::cout << "Refuel tile touched" << std::endl;
		pass = 1;
	} else if (tileType == TileType::Hazard)
	{
		std::cout << "Hazard tile touched" << std::endl;
	} else if (tileType == TileType::Victory)
	{
		std::cout << "Victory tile touched" << std::endl;
	}

}
