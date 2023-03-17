#include "stdafx.h"
#include "Action.h"
#include"Switch.h"

using namespace action;

Action::Action()
{
	countArrow = 0;
}
Action::~Action()
{

}

void Action::SetName(String Name)
{
	name = Name;
}
/*
void Action::SetStart(action::Switch* Start)
{
	start << Start;
}
void Action::SetEnd(action::Switch* End)
{
	end << End;
}
void Action::AddReq(action::Switch* Req)
{
	req << Req;
}*/

void Action::conectionUpdate()
{
	for (auto ite = conections_end.begin(); ite != conections_end.end();)
	{
		if (not acts.contains(ite->first))ite = conections_end.erase(ite);
		else
		{
			for (auto ite1 = ite->second.begin(); ite1 != ite->second.end();)
			{
				if (not acts.contains(ite1->arrow_id))ite1 = ite->second.erase(ite1);
				else ite1++;
			}
			ite++;
		}
	}
	for (auto ite = conections_start.begin(); ite != conections_start.end();)
	{
		if (not acts.contains(ite->first))ite = conections_start.erase(ite);
		else
		{
			for (auto ite1 = ite->second.begin(); ite1 != ite->second.end();)
			{
				if (not acts.contains(ite1->arrow_id))ite1 = ite->second.erase(ite1);
				else ite1++;
			}
			ite++;
		}
	}	
}
