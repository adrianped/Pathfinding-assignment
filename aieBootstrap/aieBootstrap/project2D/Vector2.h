#pragma once

struct Vector2
{
	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float x;
	float y;
};