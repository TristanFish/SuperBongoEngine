#ifndef COLLIDER_H
#define COLLIDER_H

struct Collider
{
public:
	enum shape
	{
		Circle,
		Box
	}; 

	float size = 1.0f;
	shape colliderShape;
};

#endif