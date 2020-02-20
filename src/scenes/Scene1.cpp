#include "Scene1.h"
#include "custom/Player.h"

Scene1::Scene1()
{}

Scene1::~Scene1()
{}

bool Scene1::OnCreate()
{
	Shader shader;
	camera;
	
	std::cout << "scene1 loaded" << std::endl;

	//Setup the player
	player = new Player("Player1", MATH::Vec3(0.0f, 0.0f, 0.0f));
	if (player->hasComponent<SpriteComponent>())
	{
		shader.TakeInUniformMat4("viewProjectionMatrix", camera->getProjectionMatrix());
		//Give it a shader and a sprite
		player->getComponent<SpriteComponent>().setShaders("src/graphics/ShaderVert.glsl", "src/graphics/ShaderText.glsl");
		player->getComponent<SpriteComponent>().setTexture("src/test.jpg");
	}
	std::cout << camera->getProjectionMatrix() << std::endl;
	
	//Init camera


	//add it to the list of gameobjects
	objectList.addGameObject(player);

	//This init function separates any gameobjects that have rigidbodies for their physics calculations
	objectList.Init();

	return false;
}



void Scene1::OnDestroy()
{
	

}

void Scene1::Update(const float deltaTime)
{
	objectList.Update(deltaTime);
}



void Scene1::Render() const
{
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objectList.Render();

}

void Scene1::HandleEvents(const SDL_Event& event)
{
	objectList.HandleEvents(event);
}
