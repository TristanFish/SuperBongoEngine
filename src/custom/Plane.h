
#include "components/ECS.h"
#include "components/Components.h"

class Plane : public GameObject
{

public:
	Plane(const char* name, MATH::Vec3 position);
	~Plane() override = default;
	
	void operator()(RigidBody3D& otherBody) override;
	void OnCollisionEnter(RigidBody3D& otherBody);

	const char* GetClassIDName() const override;

private:
};

