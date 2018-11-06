// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();	
}

// ---------------------------------------------
void j1Timer::Start()
{
	// TODO 1: Fill Start(), Read(), ReadSec() methods
	// they are simple, one line each!
	StartTime = SDL_GetTicks();
	
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	return SDL_GetTicks()-StartTime;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	return(float ((SDL_GetTicks()-StartTime)/1000));
}