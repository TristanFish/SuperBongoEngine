#ifndef VECTOR_H
#define VECTOR_H
#include <cmath>
#include <iostream>
#include <string>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>


/// Used for passing exceptions 
		///
		/// Vec3 definitions followed by Vec4 
		/// There are notes at the bottom of this file you might want to read
		///

namespace MATH
{
/// This is used in normalizing vectors. Dividing by zero is a well known
/// problem but dividing by nearly zero is also a problem. 1.0x10-7 is very
/// small in "float" percision. 

	#ifndef VERY_SMALL
	#define VERY_SMALL 1.0e-7f
	#endif

	#ifndef M_PI
	#define M_PI 3.14159265358979323846f
	#endif

	#ifndef DEGREES_TO_RADIANS
	#define DEGREES_TO_RADIANS (M_PI / 180.0f)
	#endif

	#ifndef RADIANS_TO_DEGREES
	#define RADIANS_TO_DEGREES (180.0f / M_PI)
	#endif



	struct Vec2
	{
		float x, y;
	
		Vec2()
		{
			x = 0.0f;
			y = 0.0f;
		}

		Vec2(float _default)
		{
			x = _default;
			y = _default;
		}

		Vec2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		Vec2 operator / (const Vec2& v) const {
			return Vec2(x / v.x, y / v.y);
		}

		bool operator != (const Vec2& v) const {
			return (x != v.x || y != v.y);
		}

		inline const bool operator == (const Vec2& v) const {
			return (x == v.x && y == v.y);
		}

		inline const Vec2 operator + (const Vec2& v) const
		{
			return Vec2(x + v.x, y + v.y);
		}
		Vec2 operator - (const Vec2& v) const
		{
			return Vec2(x - v.x, y - v.y);
		}
		
		Vec2  operator * (const float s) const {
			return Vec2(s * x, s * y);
		}
		Vec2  operator / (const float s) const {
			return Vec2(x / s, y / s);
		}

		Vec2  operator - (const float s) const {
			return Vec2(s - x, s - y);
		}
	};

	struct Vec3 {
		float  x,y,z;	///  Structures are default public



		void set(float x_, float y_, float z_)
		{
			x = x_; y = y_; z = z_;
		}
		/// Here's a set of constructors
		explicit Vec3(float s = 0.0f){
			x = s;
			y = s;
			z = s;
		}

		Vec3( float x, float y, float z ){
			this->x = x;
			this->y = y;
			this->z = z;
		}
		
		Vec3(const Vec2& v) {
			x = v.x;
			y = v.y;
			z = 0.0f;
		}

		/// A copy constructor
		Vec3( const Vec3& v ) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		
		///////////////////////////////////////////////////////////
		/// Operator overloads (see note 1 at the end of this file)
		///////////////////////////////////////////////////////////

		/// An assignment operator   
		Vec3& operator = (const Vec3& v) = default;
		
		const bool operator == (const Vec3& v) const {
			return (x == v.x && y == v.y && z == v.z);
		}

		/// Now we can use the Vec3 like an array but we'll need two overloads
		float operator [] ( int index) const {  /// This one is for reading the Vec3 as if where an array
			return *(&x + index); 
		}

		float& operator [] ( int index ) {	/// This one is for writing to the Vec3 as if where an array.  
			return *(&x + index);					/// See note 2 at the end of this file about lvalues and rvalues
		}
	
		/// Add two Vec3s
		Vec3 operator + ( const Vec3& v ) const { 
			return Vec3( x + v.x, y + v.y, z + v.z ); 
		}

		/// Add a Vec3 to itself
		Vec3& operator += ( const Vec3& v ){ 
			x += v.x;  
			y += v.y;  
			z += v.z;  
			return *this; 
		}

		/// Take the negative of a Vec3
		Vec3 operator - () const  { 
			return Vec3( -x, -y, -z ); 
		}   

		/// Subtract two Vec3s
		Vec3 operator - ( const Vec3& v ) const { 
			return Vec3(x - v.x, y - v.y, z - v.z ); 
		}

		/// Subtract a Vec 3 from itself
		Vec3& operator -= ( const Vec3& v ){ 
			x -= v.x;  
			y -= v.y;  
			z -= v.z;  
			return *this;
		}

		/// Multiply a Vec3 by a scalar
		Vec3  operator * ( const float s ) const { 
			return Vec3(s*x, s*y, s*z ); 
		}
		
		/// Multiply a scalar by a Vec3   It's the scalar first then the Vec3
		/// Overloaded and a friend, ouch! It's the only way to make it work with a scalar first.
		/// Friends are tricky, look them up. 
		friend Vec3 operator * ( const float s, const Vec3& v ) { 
			return v * s; 
		}

		/// Multiply a Vec3 by a scalar and assign it to itself
		Vec3& operator *= ( const float s ) { 
			x *= s; 
			y *= s;  
			z *= s;  
			return *this; 
		}

		/// Multiply a Vec3 by a scalar and assign it to itself
		inline Vec3& operator *= (const Vec3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		/// Divide by a scalar - Watch for divide by zero issues
		Vec3 operator / ( const float s ) const {
	#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( fabs(s) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero! ");
			throw errorMsg;
		}
	#endif
		float r = 1.0f / s;
		return *this * r;
		}

		Vec3& operator /= ( const float s ) {
#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( std::fabs(s) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero! ");
			throw errorMsg;
		}
#endif // DEBUG
		float r = 1.0f / s;
		*this *= r;
		return *this;
		}

		static Vec3 Forward()
		{
			return Vec3(0.0f, 0.0f, 1.0f);
		}
		
		static Vec3 Up()
		{
			return Vec3(0.0f, 1.0f, 0.0f);
		}
		
		static Vec3 Right()
		{
			return Vec3(1.0f, 0.0f, 0.0f);
		}

		
		
		void print() const { 
			printf("%1.8f %1.8f %1.8f\n", x,y,z);		  
		}

		///
		/// Type conversion operators 
		///
		operator const float* () const {
			return static_cast<const float*>(&x);
		}
		

		operator float* () {
			return static_cast<float*>(&x);
		}

		std::string ToString() const
		{
			using namespace std;
			string s = to_string(x);
			s.append(", ");
			s.append(to_string(y));
			s.append(", ");
			s.append(to_string(z));
			
			return s;
		}

		//D serialize Vec3
		template<class Archive>
		void serialize(Archive& archive) {
			archive(cereal::make_nvp("X" , x), cereal::make_nvp("Y", y), cereal::make_nvp("Z", z));
		}
		
		

	};
	std::ostream& operator<< (std::ostream& out, const Vec3& v);


		/// Vec4 definitions
		/// I am intentionally creating a Vec4 from a Vec3 so I can pass a Vec4 into a Subroutine that wants a Vec3
		/// in many cases this will be mathamatically OK, just be careful Vec4's are not quaterinians
		
	struct Vec4: public Vec3 {
		///float  x;	///
		///float  y;	///  
		///float  z;	/// From Vec3
		float  w;

		/// Here's a set of constructors
		explicit Vec4( float s = 0.0f ){ x=s; y=s; z=s; w=s;}
		Vec4( float _x, float _y, float _z, float _w){ x=_x; y=_y; z=_z; w=_w;} 
		Vec4( const Vec4& v ) { 
			x = v.x;  
			y = v.y;  
			z = v.z; 
			w = v.w;
		}

		Vec4( const Vec3& v ) { 
			x = v.x;  
			y = v.y;  
			z = v.z; 
			w = 1.0f;
		}

		Vec4(const Vec3& v,const float& w_) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = w_;
		}
		
		/// An assignment operator
		Vec4& operator = (const Vec4& v){
			x = v.x;  
			y = v.y;  
			z = v.z; 
			w = v.w;
			return *this;
		}

		


		/// See Vec3 definition 
		float& operator [] ( int index ) { 
			return *(&x + index); 
		}
		float operator [] ( int i ) const { 
			return *(&x + i); 
		}

		/// See Vec3 definition 
		Vec4 operator + ( const Vec4& v ) const { 
			return Vec4( x + v.x, y + v.y, z + v.z, w + v.w ); 
		}

		/// See Vec3 definition 
		Vec4& operator += ( const Vec4& v ){ 
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this; 
		}

		//// See Vec3 definition 
		Vec4 operator - () const  { 
			return Vec4( -x, -y, -z, -w );
		}   

		/// See Vec3 definition 
		Vec4 operator - ( const Vec4& v ) const { 
			return Vec4( x - v.x, y - v.y, z - v.z, v.w - w);
		}

		/// See Vec3 definition 
		Vec4& operator -= ( const Vec4& v ){ 
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		/// See Vec3 definition 
		Vec4 operator * ( const float s ) const { 
			return Vec4( s*x, s*y, s*z, s*w);
		}

		/// See Vec3 definition 
		Vec4& operator *= ( const float s ) { 
			x *= s;
			y *= s;
			z *= s;
			w*= s;
			return *this;
		}

		/// See Vec3 definition 
		friend Vec4 operator * ( const float s, const Vec4& v ) { 
			 return v * s; 
		}


		Vec4 operator / ( const float s ) const {
#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( std::fabs(s) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero! ");
			throw errorMsg;
		}
#endif
		float r = 1.0f / s;
		return *this * r;
		}

		Vec4& operator /= ( const float s ) {
	#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( std::fabs(s) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero! ");
			throw errorMsg;
		}
	#endif // DEBUG

		float r = 1.0f / s;
		*this *= r;
		return *this;
		}

		void Print() const { 
			printf("%1.8f %1.8f %1.8f %1.8f\n", x,y,z,w);		  
		}

		///
		/// Type conversion operators 
		///
		operator const float* () const { 
			return static_cast<const float*>( &x );
		}

		operator float* () { 
			return static_cast<float*>( &x );
		}

		//D serialize Vec4
		template<class Archive>
		void serialize(Archive& archive) {
			archive(cereal::make_nvp("X", x), cereal::make_nvp("Y", y), cereal::make_nvp("Z", z), cereal::make_nvp("W", w));
		}

	};
}


#endif


		/*** Note 1.
		I know, I hate operator overloading as a general rule but a few make sense!! Just be careful and 
		consistent. In the following code, I will overload many operators. I don't believe in 
		overloading when the operator is less than obvious.  
		For example, in this class, the relational operators (== != < > <= >=) might mean "in relation 
		to their magnitude or direction" I'm just not sure. Just write a function to do that and don't make 
		me guess what the operator might mean. Use the idea of "Least Astonishment" don't surprise me,
		or shock me or anything else that will piss me or others off-SSF
		***/

		/*** Note 2.
		In straight C programming, lvalues and rvalues literially means left and right assignments.  For example, 
		int x = 5; x is the lvalue and 5 is the rvalue. Easy. In C++ it get a bit more tricky. The modern idea is
		rvalues are temporary vaules residing in the registers of the CPU. lvalues are actual memory locations.  
		In the code:
			inline float& operator [] ( int index ) {	
				return *(&x + index);					
			}
		To read this precicely, &x is the address of the x variable (the first in the list of x,y,z) add to that 
		address the index as if it were an array. Then *(&x + index) dereferences that address back into the 
		object, the float& reference returns it as an address and thus an lvalue.
		***/

		/*** Note 3.
		The issue here is that I need to use functions in Vector from VMath which require Vector but VMath
		also requires Vector - this is a classic circular dependency problem or sometimes known as mutual recursion.
		To solve this it requires a forward declaration.  A "forward declaration" is the declaration of a class for which 
		the you have not yet given a complete definition of (whateverClass.h). To do this all you need is the statement:  
		"class VMath;"  This warns the compiler that I will be using VMath in Vector; HOWEVER, in this case, it won't work because
		I'm using the vector.h as a totally inlined set of functions - sometimes you're just screwed 
		*/
