#ifndef INSTANCER_H
#define INSTANCER_H
#include "components/ECS.h"
#include "components/Components.h"
class Instancer
{
	
public:
	void Init(const unsigned int& amount_, GameObject* g);
	Instancer();
	~Instancer();
	
	virtual void CalculateModelMatrices(const Transform& transform, const unsigned int instanceAmount);
private:
	MATH::Matrix4* modelMatrices;
	void BindBuffers(const MeshRenderer& renderer, const unsigned int instanceAmount)const;

};

#endif

