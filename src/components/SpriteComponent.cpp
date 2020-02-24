#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const char* path)
{
	texture = new TextureManager();
	texture->LoadImage(path);
}

SpriteComponent::~SpriteComponent()
{
	delete texture;
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SpriteComponent::setTexture(const char* path)
{
	texture->LoadImage(path);
}

void SpriteComponent::setShaders(const char* vertexPath, const char* fragmentPath)
{
	 shader.CreateShader(vertexPath, fragmentPath);
}

void SpriteComponent::Init(GameObject *g)
{
	gameobject = g;
	texture = new TextureManager();

	float vertices[] = {
		// positions           // colors          // texture coords
		 0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,    0.0f,0.0f,1.0f,       0.0f, 0.0f,
		-0.5f,  0.5f,  0.0f,   1.0f, 1.0f, 0.0f,     0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3, //first triangle 
		1, 2, 3  //second triangle
	};

	std::cout << "scene1 loaded" << std::endl;
	//Naming our buffer and array objects
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);


	//Binding the objects to say that "hey you are going to be used"


	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Binding the vertices, we draw it as static instead of dyanmic because it wont be updating much for now


	//The following function call takes parameters for where to start the array, the size of the attribute (3 because its a vec3), the data type, if it is normalized, the size of the stride, and the offset. 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void SpriteComponent::Update(const float deltaTime)
{

}

void SpriteComponent::Render() const
{
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	shader.RunShader();
	   
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//This says what we are going to draw and how many things we are going to draw

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SpriteComponent::HandleEvents(const SDL_Event& event)
{

}
