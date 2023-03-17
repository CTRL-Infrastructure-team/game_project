#include "stdafx.h"
#include "c_timer.h"

void c_timer::update(bool stop)
{
	if (not stop)
	{
		time_s += Scene::DeltaTime();
		periodTimer += Scene::DeltaTime();
		LimitTimer += Scene::DeltaTime();
		Limit();
	}
}

bool c_timer::Period()
{	
	if (periodTimer > periodTime)
	{
		periodTimer -= periodTime;
		return true;
	}

	return false;
}
