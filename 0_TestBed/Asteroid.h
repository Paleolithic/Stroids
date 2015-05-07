#pragma once

#include "ME\Mesh\PrimitiveManagerSingleton.h"
using namespace MyEngine;

class Asteroid
{
	vector3 start_position;

public:
	Asteroid(void);

	/*
	Constructor - Set starting position, speed, and direction
	*/
	Asteroid(vector3 position, float spd, bool right);
	~Asteroid(void);

	float life_time;
	float screen_percentage;
	float speed;
	bool go_right;

	//Nah man fuck that, just access them directly like a boss
	void SetLT(float lt);
	void SetSP(float sp);
	float GetLT();
	float GetSP();
};

