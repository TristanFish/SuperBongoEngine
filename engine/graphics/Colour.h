#pragma once
#include <algorithm>


struct Colour
{
	float r, g, b, a;

	Colour()
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 1.0f;
	}

	Colour(float r_, float g_, float b_)
	{
		r = std::clamp(r_, 0.0f, 1.0f);
		g = std::clamp(g_, 0.0f, 1.0f);
		b = std::clamp(b_, 0.0f, 1.0f);
		a = 1.0f;
	}
	
	Colour(float r_, float g_, float b_,float a_)
	{
		r = std::clamp(r_, 0.0f, 1.0f);
		g = std::clamp(g_, 0.0f, 1.0f);
		b = std::clamp(b_, 0.0f, 1.0f);
		a = std::clamp(a_, 0.0f, 1.0f);
	}
};

