#pragma once

class PhysicsBox;

//これを継承してブロックを作る
class Object
{
public:
	String name;
	virtual void reaction(Point pos, PhysicsBox* box) {}
	virtual void draw(Point pos) {}
};
