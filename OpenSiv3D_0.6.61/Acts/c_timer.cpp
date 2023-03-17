#include "../stdafx.h"
#include "c_timer.h"

void c_timer::update(bool stop)
{
	if (not stop)
	{
		if (already_updated_in_this_SystemUpdate.second != *SystemUpdate_id)
		{
			already_updated_in_this_SystemUpdate.first = false;
			already_updated_in_this_SystemUpdate.second = *SystemUpdate_id;
		}
		if (not already_updated_in_this_SystemUpdate.first) {
			time_s += Scene::DeltaTime();
			periodTimer += Scene::DeltaTime();
			LimitTimer += Scene::DeltaTime();
			Limit();
			already_updated_in_this_SystemUpdate.first = true;
			
		}
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
