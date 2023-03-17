#pragma once
#include"../Character.h"

class Player:public Character
{
public:
	Player(Vec2 pos, Vec2 vel, Vec2 acc, RectF hitbox, Array<Character*>* list, bool* SystemUpdate);
	void update();
	void draw2();
private:
	double walk_speed;
	HashTable<String, Texture> texture;
	String state;
};
