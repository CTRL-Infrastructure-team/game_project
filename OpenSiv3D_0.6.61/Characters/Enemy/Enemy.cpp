#include "../../stdafx.h"
#include "Enemy.h"
#include"../../Acts/Act/TextureDraw.h"
#include"../../Acts/Act/Move.h"
#include"../../Acts/Act/Attack.h"

Snake::Snake(HitBox hitbox, Array<Character*>* List, bool* SystemUpdate, Vec2 init_pos, Vec2 init_vel, Vec2 init_acc)
	:Enemy(HitBox{ new Vec2{},new Vec2{},hitbox.Get_Box() }, init_pos, init_vel, init_acc, List, SystemUpdate)
{
	Move* jump;
	Attack* jump_attack;
	jump= new Move(&pos, &direction);
	jump->UniformlyAcceleratedMotion(&vel,&acc, Vec2{ 270,-500 }, Vec2{ 0,100 });
	jump_attack = new Attack(HitBox{ new Vec2{},new Vec2{},hitbox.Get_Box() }, &power, this, list, player);
	double scale1 , scale2;
	scale1 = 0.2; scale2 = 0.25;

	actions->ReadJSON(U"../Characters/ActionJSON/Snake_action.json");
	actions->SetAct(U"Attack", U"stop", new Stop(&vel));
	actions->SetAct(U"Attack", U"attack", jump_attack);
	actions->SetAct(U"Attack", U"jump", jump);
	actions->SetAct(U"Attack", U"land", new Stop(&vel));
	/*actions->SetAct(U"Attack", U"stopTexture", new TextureDraw(Texture{ U"Texture/CandySnake_500_500.png" }, this, scale1));
	actions->SetAct(U"Attack", U"jumpTexture", new TextureDraw(Texture{ U"Texture/CandySnake2_500_500.png" },this,scale2));
	actions->SetAct(U"Attack", U"nomalTexture", new TextureDraw(Texture{ U"Texture/CandySnake1_500_500.png" }, this, scale2));*/
	actions->Set_all();
	SensorShape = RectF{ Arg::center(60,-30),200,150 };
	Sensor = new HitBox(&pos, &vel, SensorShape);
	nomal = Texture{ U"Texture/CandySnake1_500_500.png" };
}

void Snake::update()
{
	if (actions->Active(U"Attack"))
	{
		if (box.touch(down_line)) { actions->EndAct(U"Attack", U"jump"); }
	}
	else
	{
		//敵を見つける
		if (box.touch(down_line))
		{
			for (const auto& chara : *list)
			{
				if (chara->Type() == player)
				{
					if (Sensor->intersects(chara->box))
					{
						//標的の方を向く
						if (pos.x < chara->pos.x)direction = Vec2{ 1,0 };
						else direction = Vec2{ -1,0 };
						actions->Start(U"Attack");
					}
				}
			}
		}

		if (direction == Vec2{ -1,0 })
		{
			RectF Shape = SensorShape;
			Shape.setCenter(-Shape.center().x, Shape.center().y);
			Sensor->SetShape(Shape);
		}
		else if (direction == Vec2{ 1,0 })
		{
			Sensor->SetShape(SensorShape);
		}

		if (box.touch(right_line))
		{
			//右に衝突したら左に
			direction = Vec2{ -1,0 };
		}
		if (box.touch(left_line))
		{
			//左に衝突したら右に
			direction = Vec2{ 1,0 };
		}
		
		vel.x = 100 * direction.x;
	}

	if (box.touch(up_line) && vel.y < 0)vel.y = 0;//天井に衝突したらy軸速度を0に
	if (box.touch(down_line) && vel.y > 0)vel.y = 0;//床に衝突したらy軸速度を0に

	Sensor->update();
}

void Snake::draw2()
{
	if (not actions->Active(U"Attack"))
	{
		if (direction == Vec2{ 1,0 })nomal.scaled(0.25).drawAt(pos); else nomal.mirrored().scaled(0.25).drawAt(pos);
		Sensor->draw(ColorF{ Palette::Yellow,0.3 });
	}
}
