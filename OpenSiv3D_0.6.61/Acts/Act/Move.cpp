#include"../../stdafx.h"
#include"Move.h"
#include"../../Characters/Character.h"

void Move::SetInitPos(Vec2 init)
{
	use_init_pos = true;
	init_pos = init;
}

void Move::SetInitVel(Vec2 init)
{
	use_init_vel = true;
	init_vel = init;
}

void Move::SetInitAcc(Vec2 init)
{
	use_init_acc = true;
	init_acc = init;
}

void Move::SetFunction(void (*func)(Vec2* Pos, Vec2* Vel, Vec2* Acc, double t), Vec2* pos, Vec2* vel, Vec2* acc)
{
	p_pos = pos; p_vel = vel; p_acc = acc;
	function = func;
	use_function = true;
}

void Move::UniformlyAcceleratedMotion(Vec2* p_vel, Vec2* p_acc,Vec2 acc, Vec2 vel)
{
	AdditionInitVector(p_vel, p_acc);
	SetInitVel(vel);
	SetInitAcc(acc);
	noupdate = true;
}

void Move::endAct()
{
	if(endact)
	{
		c_acc = Vec2{ 0,0 };
		c_vel = Vec2{ 0,0 };
		first = true;
	}
}

void Move::startAct()
{
	if (first)
	{
		bool flag=false;
		if (direction)
		{
			if (*Chara_Direction == Left)
			{
				flag = true;
				init_vel.x = -init_vel.x;
				init_acc.x = -init_acc.x;
			}
		}
		if (((not use_function) and init_vector_addition)or noupdate)
		{
			if (use_init_pos)*p_pos += init_pos;
			if (use_init_vel)*p_vel += init_vel;
			if (use_init_acc)*p_vel += init_acc;
		}
		else
		{
			if (use_init_pos)*p_pos = init_pos;
			if (use_init_vel)c_vel = init_vel;
			if (use_init_acc)c_acc = init_acc;
		}
		first = false;
		if (flag)
		{
			init_vel.x = -init_vel.x;
			init_acc.x = -init_acc.x;
		}
	}
}

void Move::update()
{
	if (use_function)
	{
		function(p_pos, p_vel, p_acc, timer->time());
	}
	else if(not noupdate)
	{
		c_vel += c_acc * Scene::DeltaTime();
		*p_pos += c_vel * Scene::DeltaTime();
	}
}

void UniformLinearMotion::startAct()
{
	if (direction == Vec2{ -1,0 })
	{
		if (not chara->box.touch(left_line))
		{
			chara->vel.x -= speed_per_s;
			chara->direction = Left;
		}
	}
	else if (direction == Vec2{ 1,0 })
	{
		if (not chara->box.touch(right_line))
		{
			chara->vel.x += speed_per_s;
			chara->direction = Right;
		}
	}
	else if (direction == Vec2{ 0,-1 })
	{
		if (not chara->box.touch(up_line))
		{
			chara->vel.y -= speed_per_s;
			chara->direction = Vec2{ 0,-1 };
		}
	}
	else if (direction == Vec2{ 0,1 })
	{
		if (not chara->box.touch(down_line))
		{
			chara->vel.y += speed_per_s;
			chara->direction = Vec2{ 0,1 };
		}
	}
}

void UniformLinearMotion::update()
{
	if (direction == Vec2{ -1,0 })
	{
		if (chara->box.touch(left_line))
		{
			End();
		}
	}
	else if (direction == Vec2{ 1,0 })
	{
		if (chara->box.touch(right_line))
		{
			End();
		}
	}
	else if (direction == Vec2{ 0,-1 })
	{
		if (chara->box.touch(up_line))
		{
			End();
		}
	}
	else if (direction == Vec2{ 0,1 })
	{
		if (chara->box.touch(down_line))
		{
			End();
		}
	}
}

void UniformLinearMotion::endAct()
{
	if (do_endAct)
	{
		if (direction == Vec2{ -1,0 } or direction == Vec2{ 1,0 })
		{
			chara->vel.x = 0;
		}
		else if (direction == Vec2{ 0,-1 } or direction == Vec2{ 0,1 })
		{
			chara->vel.y = 0;
		}
	}
}

void Jump::SetJump(double g,double height)
{
	gravity = g;
	speed = Math::Sqrt(2 * g * height);
}

void Jump::startAct()
{
	chara->vel.y = -speed;
	chara->acc.y = gravity;
}

void Jump::update()
{
	if (chara->box.touch(up_line))
	{
		chara->vel.y = 0;
		End();
	}
	if (chara->vel.y * chara->acc.y >= 0)
	{
		End();
	}
}

void Jump::endAct()
{
	chara->acc.y = chara->Get_G();
}

void Fall::SetG(double g)
{
	gravity = g;
}

void Fall::SetFirstV(double s)
{
	speed = s; use_speed = true;
}

void Fall::startAct()
{
	if (use_speed)chara->vel.y = speed;
	chara->acc.y = gravity;
}

void Fall::update()
{
	if (chara->box.touch(down_line))
	{
		chara->vel.y = 0;
		End();
	}
}

void Fall::endAct()
{
	chara->acc.y = chara->Get_G();
}

Slip::Slip(Character* chara, double d) :chara(chara)
{
	move = new Move(&chara->pos, &chara->direction);
	Deceleration(d);
}

Slip::~Slip() {}

void Slip::Deceleration(double d)
{
	deceleration = d;
}

void Slip::startAct()
{
	Vec2 vel = chara->vel;
	vel.setLength(deceleration);
	vel.rotate(Math::Pi);
	acc = vel;
}

void Slip::update()
{
	double inner_product;
	inner_product = chara->vel.x * acc.x + chara->vel.y * acc.y;
	//速度と加速度の向きがそろうと終了
	if (inner_product<0)
	{
		chara->vel += acc * Scene::DeltaTime();
	}
	else
	{
		End();
	}
}

void Slip::endAct()
{
	chara->vel.x = 0;
}
