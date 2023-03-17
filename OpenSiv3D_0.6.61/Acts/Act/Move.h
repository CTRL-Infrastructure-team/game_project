#pragma once
#include"Act.h"
#include "../../HitBox.h"

class Move :public Act
{
public:
	Move(Vec2* pos) :p_pos(pos) { c_vel = c_acc = Vec2{ 0,0 }; direction = false; };
	//chara_Directionによって方向を変える。初速度などを決めるとき、右向きを想定する。
	Move(Vec2* pos, Vec2* chara_Direction):p_pos(pos), Chara_Direction(chara_Direction)
	{
		c_vel = c_acc = Vec2{ 0,0 }; direction = true;
	}
	~Move() {};
	void SetInitPos(Vec2 init);
	void SetInitVel(Vec2 init);
	void SetInitAcc(Vec2 init);
	//InitPos,InitVel,InitAccは代入ではなく加算になる↓
	void AdditionInitVector(Vec2* vel, Vec2* acc) { init_vector_addition = true; p_vel = vel, p_acc = acc; };
	void SetFunction(void(*func)(Vec2* Pos, Vec2* Vel, Vec2* Acc, double t), Vec2* pos, Vec2* vel, Vec2* acc);
	void UniformlyAcceleratedMotion(Vec2* vel, Vec2* acc,Vec2 init_vel, Vec2 init_acc);
	void startAct();
	void endAct();

	void update();

	Vec2 c_vel, c_acc;
private:
	Vec2* Chara_Direction;
	bool direction;
	void (*function)(Vec2* Pos, Vec2* Vel, Vec2* Acc,double t);
	bool use_function = false;
	Vec2 init_pos, init_vel, init_acc;
	Vec2* p_pos, * p_vel, * p_acc;
	Vec2 end_vel;
	bool use_init_pos = false;
	bool use_init_vel = false;
	bool use_init_acc = false;
	bool init_vector_addition = false;
	bool first = true;
	bool noupdate = false;
};

class Character;

//壁にぶつかると勝手に終了
class UniformLinearMotion :public Act
{
public:
	UniformLinearMotion(Character* chara,double speed_per_s,Vec2 Direction) :chara(chara),speed_per_s(speed_per_s)
	{
		Direction.setLength(1);
		direction = Direction;
	}
	~UniformLinearMotion()
	{

	}
	void startAct();

	void update();

	void endAct();

	bool do_endAct = true;
private:
	Character* chara;
	double speed_per_s;
	Vec2 direction;
};
//y軸速度が0になると自動で終了
class Jump :public Act
{
public:
	Jump(Character* chara) :chara(chara) {};
	~Jump() {};
	//ジャンプ時の重力加速度とジャンプの高さ
	void SetJump(double gravity, double height);
	void startAct();
	void update();
	void endAct();
private:
	Character* chara;
	double gravity;
	double speed;//初速度
};

class Fall :public Act
{
public:
	Fall(Character* chara) :chara(chara) {

	};
	~Fall() {};
	//重力加速度と初速度(下方向)
	void SetG(double gravity);
	void SetFirstV(double speed);
	void startAct();
	void update();
	void endAct();
private:
	Character* chara;
	double gravity;
	double speed;//初速度
	bool use_speed=false;
};
//開始時のcharaの向きによって方向が変わる。速度が0になったら自動で終了する。
class Slip :public Act
{
public:
	Slip(Character* chara, double d = 0);
	~Slip();

	void Deceleration(double d);//一秒間にdピクセル減速

	void startAct();

	void update();

	void endAct();
private:
	Move* move;
	Character* chara;
	double deceleration;
	Vec2 acc;
};
//最初に速度を0にするだけ
class Stop :public Act
{
public:
	Stop(Vec2* vel):vel(vel) {}
	~Stop() {}

	void startAct() { *vel = Vec2{ 0,0 }; };
private:
	Vec2* vel;
};
