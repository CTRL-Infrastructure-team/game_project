#include "../../stdafx.h"
#include "Act.h"

void Act::Start(double t)
{
	timer->timeRestart(t);
	startAct();
	startAct1();
}

void Act::unUse_endact()
{
	endact = false;
}

void Act::update1()
{
	timer->update();
}

void Act::End()
{
	end1 = true;
}
