#ifndef COLLIDER_H
#define COLLIDER_H

struct Collider
{
public:
	static enum shape
	{
		Circle,
		Box
	}; 

	float size;
	shape colliderShape;
};

#endif