#include "Asteroid.h"


Asteroid::Asteroid(void)
{
	life_time = 0.0f;
	screen_percentage = 0.0f;
	speed = 1.0f;
	scale = 1.0f;
	go_right = true;
	colliding = false;
	isSlowed = false;
}

Asteroid::Asteroid(vector3 position, float spd, float scl, bool right)
{
	start_position = position;
	life_time = 0.0f;
	screen_percentage = 0.0f;
	speed = spd;
	scale = scl;
	go_right = right;
	colliding = false;
	isSlowed = false;
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
