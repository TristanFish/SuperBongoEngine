#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "Vector.h"

class GameObject;

using namespace MATH;

namespace Dynamic {

	class DynamicSteeringOutput {
	public:
		Vec3 linearVel;
		Vec3 angularVel;

		DynamicSteeringOutput();
		DynamicSteeringOutput(Vec3 linearVel_, Vec3 angularVel_);

		//DynamicSteeringOutput + DynamicSteeringOutput operator
		inline const DynamicSteeringOutput operator + (const DynamicSteeringOutput& v) const {
			return DynamicSteeringOutput(linearVel + v.linearVel, angularVel + v.angularVel);
		}
		//DynamicSteeringOutput += DynamicSteeringOutput operator
		inline DynamicSteeringOutput& operator += (const DynamicSteeringOutput& v) {
			linearVel += v.linearVel;
			angularVel += v.angularVel;
			return *this;
		}
	};

	class DynamicSeek {
	private:
		GameObject* aiObject;
		GameObject* target;
	public:
		DynamicSeek(GameObject* aiObject_, GameObject* target_);
		
		//DynamicSteering 
		bool getSteering();
	};
}


#endif // !DYNAMIC_H
