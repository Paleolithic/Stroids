#include "Asteroid.h"


Asteroid::Asteroid(void)
{
	life_time = 0.0f;
	screen_percentage = 0.0f;
	speed = 1.0f;
	rand_Y = 0.0f;
	go_right = true;
	aBO = nullptr;
	colliding = false;
	isNearShip = false;
	isSlowed = false;
}

Asteroid::Asteroid(float spd, String nm, bool right,BoundingObjectClass* bo)
{
	life_time = 0.0f;
	screen_percentage = 0.0f;
	speed = spd;
	rand_Y = 0.0f;
	name = nm;
	go_right = right;
	rand_Y = 0.0f;
	aBO = bo;
	colliding = false;
	isNearShip = false;
	isSlowed = false;
}

Asteroid::Asteroid(const Asteroid& otherAst)
{
	life_time			= otherAst.life_time;
	screen_percentage	= otherAst.screen_percentage;
	speed				= otherAst.speed;
	rand_Y				= otherAst.rand_Y; 
	name				= otherAst.name;
	go_right			= otherAst.go_right;
	aBO					= otherAst.aBO;
	colliding			= otherAst.colliding;
	isNearShip			= otherAst.isNearShip;
}

Asteroid::~Asteroid(void)
{
}

void Asteroid::SetLT(float lt){
	life_time = lt;
}

void Asteroid::SetSP(float sp){
	screen_percentage = sp;
}

float Asteroid::GetLT(){
	return life_time;
}

float Asteroid::GetSP(){
	return screen_percentage;
}

void Asteroid::SetBO(BoundingObjectClass* bo){
	aBO = bo;
}