#pragma once
#include "Act.h"
#include "../../HitBox.h"
#include"../../Characters/Character.h"
#include"../Acts.h"

//指定した場所に攻撃判定を出す。
class Attack :public Act
{
public:
	//Characterの向きによって攻撃方向を変える。
	//HitBoxは{new Vec2{0,0},new Vec2{0,0},"図形"}で渡してください。
	//"図形"の座標はattackerからの相対位置になります。
	Attack(HitBox hitbox, double* power,Character* attacker, Array<Character*>* list ,CharaType target);
	~Attack();

	void startAct();

	void update();

	void endAct();

	void draw();//hitboxを描く

	bool drawhitbox = false;
protected:
	HitBox hitbox;
	double *power;
	Character* attacker;
	Array<Character*>* list;
	CharaType target;
	Vec2 FirstRelative;
};

