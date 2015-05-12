#pragma once

#include "ME\Mesh\PrimitiveManagerSingleton.h"
#include "ME\MyEngine.h"
using namespace MyEngine;

class Asteroid
{

public:
	Asteroid(void);

	/*
	Constructor - Set starting position, speed, and direction
	*/
	Asteroid(float spd, String nm, bool right, BoundingObjectClass* bo);
	Asteroid(const Asteroid& otherAst);
	~Asteroid(void);

	float life_time;
	float screen_percentage;
	float speed;
	String name;
	bool go_right;
	float rand_Y;
	BoundingObjectClass* aBO;
	bool colliding;
	bool isNearShip;

	//Nah man fuck that, just access them directly like a boss
	void SetLT(float lt);
	void SetSP(float sp);
	float GetLT();
	float GetSP();

	void SetBO(BoundingObjectClass* bo);
	void SetPosition(vector3 pos);
};

