#include "Scene1.h"
#include "custom/Player.h"
#include "tiles/Tilemap.h"

Scene1::Scene1()
{}

Scene1::~Scene1()
{
	
}

bool Scene1::OnCreate()
{
	
	std::cout << "scene1 loaded" << std::endl;
	objectList = new Manager();

	//Setup the player
	player = new Player("Player1", MATH::Vec3(0.0f, 6.0f, 0.0f));
	player->SetScale(Vec3(2.0, 2.0, 0.0));
	if (player->hasComponent<SpriteComponent>())
	{
		
		//Give it a shader and a sprite
		player->getComponent<SpriteComponent>().setShaders("src/graphics/ShaderVert.glsl", "src/graphics/ShaderText.glsl");
		player->getComponent<SpriteComponent>().setTexture("src/Textures/BallPlayer.png");
	}
	Camera::getInstance()->getProjectionMatrix().print();
	Camera::getInstance()->getViewMatrix().print();


	//add it to the list of gameobjects
	objectList->addGameObject(player);

	tilemap = new Tilemap("src/tiles/Map1.txt");

	//This init function separates any gameobjects that have rigidbodies for their physics calculations
	objectList->Init();

	return false;
}



void Scene1::OnDestroy()
{
	delete tilemap;
	tilemap = nullptr;
	delete objectList;
	objectList = nullptr;
}

void Scene1::Update(const float deltaTime)
{
	//std::cout << 1.0f / deltaTime << std::endl;
	Camera::getInstance()->Update(deltaTime);
	tilemap->Update(deltaTime);

	objectList->Update(deltaTime);
	tilemap->CheckCollisions(*player);

}



void Scene1::Render() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objectList->Render();
	tilemap->Render();

}

void Scene1::HandleEvents(const SDL_Event& event)
{
	objectList->HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}
