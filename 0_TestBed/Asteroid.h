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
	Asteroid(vector3 position, float spd, float scl, bool right);
	~Asteroid(void);

	float life_time;
	float screen_percentage;
	float speed;
	float scale;

	bool go_right;
	
	bool colliding;
	bool isSlowed;

	//Nah man fuck that, just access them directly like a boss
	void SetLT(float lt);
	void SetSP(float sp);
	float GetLT();
	float GetSP();
};

