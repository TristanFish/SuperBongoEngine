#ifndef CAMERA_H
#define CAMERA_H

#include <sdl/SDL.h>
#include "MMath.h"

class Camera
{

public:

	Camera();

	void Update(float deltaTime);
	void HandleEvents(const SDL_Event& event);


	static Camera* getInstance();
	inline static void removeInstance() { delete instance; instance = nullptr; }
	inline MATH::Matrix4& getProjectionMatrix() { return perspecProjMatrix; }
	inline MATH::Matrix4& getViewMatrix() { return viewMatrix; }
	inline void setProjectionMatrix(MATH::Matrix4 projection_) { perspecProjMatrix = projection_; }
	inline void setViewMatrix(MATH::Matrix4 view_) { viewMatrix = view_; }
	inline MATH::Vec3 getPosition() { return position; }
	inline MATH::Vec3 getRotation() { return rotation; }
	inline void setPosition(MATH::Vec3 pos) { position = pos; }
	inline void setRotation(MATH::Vec3 rot) { rotation = rot; }
		
private:
	
	static Camera* instance;
	MATH::Vec3 position;
	MATH::Vec3 rotation;
	MATH::Matrix4 rotationMatrix;
	MATH::Matrix4 viewMatrix;
	MATH::Matrix4 orthoProjMatrix;
	MATH::Matrix4 perspecProjMatrix;
	
	
};
#endif 

