#pragma once
#include"../../stdafx.h"
#include "Act.h"
//このクラスをTriggerにする
class Switch :public Act
{
public:
	Switch()
	{
	}
	~Switch() {}

	void update() {}
};
