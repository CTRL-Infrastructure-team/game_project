#pragma once
#include "../Character.h"

class Enemy :public Character
{
public:
	Enemy(Figure hitbox, Vec2 init_pos, Vec2 init_vel, Vec2 init_acc, Array<Character*>* List, bool* SystemUpdate)
		:Character(init_pos, init_vel, init_acc, CharaType::enemy, hitbox, List, SystemUpdate)
	{
		SetHitBoxColor(ColorF(Palette::Red, 0.5));
	}
	virtual ~Enemy() {}

	virtual void update() {
		if (box.touch(right_line))vel.x = -100;//右に衝突したら左に
		if (box.touch(left_line))vel.x = 100;//左に衝突したら右に
		if (box.touch(up_line) && vel.y < 0)vel.y = 0;//天井に衝突したらy軸速度を0に
		if (box.touch(down_line) && vel.y > 0)vel.y = 0;//床に衝突したらy軸速度を0に
	}
	virtual void draw2() {};
};

class Snake :public Enemy
{
public:
	Snake(Figure hitbox, Array<Character*>* List, bool* SystemUpdate, Vec2 init_pos = Vec2{ 0,0 }, Vec2 init_vel = Vec2{ 0,0 }, Vec2 init_acc = Vec2{ 0,0 });

	~Snake() {}

	virtual void update();

	virtual void draw2();
private:
	Texture nomal;
	HitBox *Sensor;
	RectF SensorShape;
};
