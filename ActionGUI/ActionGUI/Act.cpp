#include "stdafx.h"
#include "Act.h"
#include "Switch.h"

using namespace act;

Act::Act()
{

}
Act::~Act()
{
}

void Act::SetName(String Name)
{
	name = Name;
}
void Act::SetStart(Switch Start)
{
	start << Start;
}
void Act::SetEnd(Switch End)
{
	end << End;
}

