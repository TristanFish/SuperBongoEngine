#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "VMath.h"
#include "MMath.h"

#include "Collider3D.h"


using namespace MATH;


//! BoundingBox Class
/*! Handles all of the needed functionalities for a bounding box*/
class BoundingBox : public Collider3D {
	

private:

	//! Transform Matrix4
	/*! Stores the bounding boxes model matrix for converting verticies to world space*/
	Matrix4 transform;

	//! WorldVerticies vector
	/*! Stores all the verticies after being transfered to world space*/
	std::vector<Vec3> World_Verticies;

	//! Model_Verticies vector
	/*! Stores all the verticies before being transfered to world space*/
	std::vector<Vec3> Model_Verticies;

	//! B_IsWorldSpace bool
	/*! Used to figure out if the verticies have to be converted to world space again.*/
	bool B_IsWorldSpace;
public:

	//! BoundingBox Default Constructor
	/*! Initializes all default variable values*/
	inline BoundingBox() {
		transform = Matrix4();
		World_Verticies = Model_Verticies = std::vector<Vec3>();
		B_IsWorldSpace = false;
	}
	//! BoundingBox Alternate Constructor
	/*! Initializes all default variable values with the given parameters*/
	inline BoundingBox(const std::vector<Vec3>& verticies, const Matrix4& transform_) {
		World_Verticies = Model_Verticies = verticies;
		transform  = transform_;
		B_IsWorldSpace = false;

		World_Verticies.reserve(30);
		Model_Verticies.reserve(30);
	}

	inline ~BoundingBox()
	{
	}


	//! UpdateModelBounds Function
	/*! Converts the models verticies into world space and then updates them in World_Verticies vector*/
	 inline void UpdateModelBounds()
	 {
		 if (B_IsWorldSpace)
			 return;
		 // Converts verticies to world space (Assuming transform is the model matrix)
		 for (int i = 0; i < Model_Verticies.size(); i++)
		 {
			 World_Verticies[i] = transform * Model_Verticies[i];
		 }
		 B_IsWorldSpace = true;
	 }

	 //! SetWorldVerticies Function
	/*!  Updates World_Verticies and Model_Verticies with the passed in parameter.*/
	inline void SetWorldVerticies(const std::vector<Vec3>& newVerticies) 
	{ 
		World_Verticies = newVerticies;
		Model_Verticies = newVerticies;
		if (B_IsWorldSpace)
			B_IsWorldSpace = false;
	}

	//! AddWorldVertex Function
   /*!  Inserts the parameter into both World and Model verticies vector*/
	inline void AddWorldVertex(const Vec3& WorldVertex) 
	{
		World_Verticies.push_back(WorldVertex);
		Model_Verticies.push_back(WorldVertex);

		if (B_IsWorldSpace)
			B_IsWorldSpace = false;
	}

	//! SetTransform Function
   /*!  Updates the transform if it has been changed.*/
	inline void SetTransform(const Matrix4& newMatrix) 
	{ 
		if (transform == newMatrix)
			return;
		
		transform = newMatrix;
		B_IsWorldSpace = false;
	}

	//! GetTransform Getter
    /*!  Returns the transform Matrix4 by const ref*/
	inline const Matrix4& GetTransform() { return transform; }

	//!  GetWorldVerticies Getter
	/*!  Returns the World_Verticies vector by const ref*/
	inline const std::vector<Vec3>& GetWorldVerticies() { return World_Verticies; }

	//!  GetModelVerticies Getter
	/*!  Returns the Model_Verticies vector by const ref*/
	inline const std::vector<Vec3>& GetModelVerticies() { return Model_Verticies; }
};




#endif
