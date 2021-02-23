#include "SkyBox.h"
#include "core/ModelManager.h"

SkyBox::SkyBox()
{
#pragma region MyRegion
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
#pragma endregion

	bool result = LoadSkyBox("resources/skybox/right.jpg", "resources/skybox/left.jpg", "resources/skybox/top.jpg", "resources/skybox/bottom.jpg", "resources/skybox/front.jpg", "resources/skybox/back.jpg");
	if (!result) 
	{ 
		EngineLogger::Error("Skybox failed to load textures", "SkyBox.cpp", __LINE__);
	}
	
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	shader = new Shader();
	shader->CreateShader("src/graphics/shaders/SkyBoxVert.glsl", "src/graphics/shaders/SkyBoxFrag.glsl");
	shader->RunShader();
	
}

 GLuint SkyBox::GetSkyBoxTexture() const
{
	return skyboxTextureID;

}
 unsigned int SkyBox::GetVAO() const
 {
	 return skyboxVAO;
 }

SkyBox::~SkyBox()
{
	glDeleteTextures(1, &skyboxTextureID);
}
bool SkyBox::LoadSkyBox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz)
{
	
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	SDL_Surface* texSurface = IMG_Load(posx);
	if (texSurface == nullptr) { return false; }
	int mode = (texSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, texSurface->w, texSurface->h, 0, mode, GL_UNSIGNED_BYTE, texSurface->pixels);
	SDL_FreeSurface(texSurface);

	
	texSurface = IMG_Load(negx);
	if (texSurface == nullptr) { return false; }
    mode = (texSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, texSurface->w, texSurface->h, 0, mode, GL_UNSIGNED_BYTE, texSurface->pixels);
	SDL_FreeSurface(texSurface);

	
	texSurface = IMG_Load(posy);
	if (texSurface == nullptr) { return false; }
	mode = (texSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, texSurface->w, texSurface->h, 0, mode, GL_UNSIGNED_BYTE, texSurface->pixels);
	SDL_FreeSurface(texSurface);

	
	texSurface = IMG_Load(negy);
	if (texSurface == nullptr) { return false; }
	mode = (texSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, texSurface->w, texSurface->h, 0, mode, GL_UNSIGNED_BYTE, texSurface->pixels);
	SDL_FreeSurface(texSurface);

	
	texSurface = IMG_Load(posz);
	if (texSurface == nullptr) { return false; }
	mode = (texSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, texSurface->w, texSurface->h, 0, mode, GL_UNSIGNED_BYTE, texSurface->pixels);
	SDL_FreeSurface(texSurface);


	texSurface = IMG_Load(negz);
	if (texSurface == nullptr) { return false; }
	mode = (texSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, texSurface->w, texSurface->h, 0, mode, GL_UNSIGNED_BYTE, texSurface->pixels);
	SDL_FreeSurface(texSurface);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}


void SkyBox::Update(const float deltaTime)
{
	
}

void SkyBox::Render() const
{
	Matrix3 view = Camera::getInstance()->getViewMatrix();
	viewConvert = Mat3ToMat4(view);
	
	glDepthFunc(GL_LEQUAL);
	//glDisable(GL_CULL_FACE);
	shader->RunShader();
	shader->TakeUniform("viewMatrix", viewConvert);
	shader->TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
	
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
}

void SkyBox::HandleEvents(const SDL_Event& event)
{
}

Matrix4 SkyBox::Mat3ToMat4(Matrix3 _m) const
{
	Matrix4 m;
	m[0] = _m[0];   m[4] = _m[3];   m[8] = _m[6];   m[12] = 0;
	m[1] = _m[1];   m[5] = _m[4];   m[9] = _m[7];   m[13] = 0;
	m[2] = _m[2];   m[6] = _m[5];   m[10] = _m[8];   m[14] = 0;
	m[3] = 0;		m[7] = 0;		m[11] = 0;		 m[15] = 1;

	return m;
}

