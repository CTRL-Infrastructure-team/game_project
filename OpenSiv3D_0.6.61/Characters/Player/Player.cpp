#include"../../stdafx.h"
#include"Player.h"
#include"../../Acts/Act/Move.h"
#include"../../Acts/Act/Switch.h"
#include"../../Acts/Act/PrintName.h"
#include"../../Acts/Act/Attack.h"
#include"../../Acts/Act/TextureDraw.h"

Player::Player(Vec2 Pos, Vec2 Vel, Vec2 Acc, RectF hitbox, Array<Character*>* list, bool* SystemUpdate)
	:Character(Pos, Vel, Acc, player, HitBox{ &pos,&vel,hitbox }, list, SystemUpdate)
{
	walk_speed = 300;
	double jump_height = 200;
	double jump_gravity = 1000;
	double jump_fall_gravity = 1500;
	double punch_w = 60;
	double punch_h = 60;
	Vec2 punch_pos{ 30,-30 };//肩の位置
	double kick_w = 70;
	double kick_h = 60;
	Vec2 kick_pos{ 15,20 };
	Jump* jump;
	Fall* jump_fall;
	Attack* punch;
	Attack* kick;
	jump = new Jump(this); jump->SetJump(jump_gravity,jump_height);
	jump_fall = new Fall(this); jump_fall->SetG(jump_fall_gravity);
	punch = new Attack(HitBox{ new Vec2{0,0},new Vec2{0,0} ,RectF{punch_pos,punch_w,punch_h} }, & power, this, list, enemy);
	kick = new Attack(HitBox{ new Vec2{0,0},new Vec2{0,0},RectF{kick_pos,kick_w,kick_h} }, &power, this, list, enemy);
	punch->drawhitbox = true;
	kick->drawhitbox = true;
	actions->ReadJSON(U"../Characters/ActionJSON/Annna_action.json");
	actions->SetAct(U"Jumping", U"jump", jump);
	actions->SetAct(U"Jumping", U"fall", jump_fall);
	actions->SetAct(U"Punching", U"punch", punch);
	actions->SetAct(U"Punching", U"recovery", new Switch());
	actions->SetAct(U"JumpPunch", U"punch", kick);
	actions->SetAct(U"JumpPunch", U"recovery", new Switch());
	double scale=0.2;
	actions->SetAct(U"Jumping", U"JumpTexture", new TextureDraw(Texture{ U"Texture/AnnnaJump.png" }, this, scale));
	actions->SetAct(U"Punching", U"PunchTexture", new TextureDraw(Texture{ U"Texture/AnnnaAttack.png" }, this, scale));
	actions->SetAct(U"Punching", U"StandTexture", new TextureDraw(Texture{ U"Texture/AnnnaStand.png" }, this, scale));
	actions->SetAct(U"JumpPunch", U"JumpAttack", new TextureDraw(Texture{ U"Texture/AnnnaJumpPunch_500_500.png" }, this, 0.23));
	actions->SetAct(U"JumpPunch", U"JumpTexture", new TextureDraw(Texture{ U"Texture/AnnnaJump.png" }, this, scale));
	actions->Set_all();
}

void Player::update()
{
	if (box.touch(down_line))actions->End(U"JumpPunch");

	if (MouseL.down())
	{
		if (not box.touch(down_line))
		{
			actions->EndAct(U"Jumping", U"JumpTexture");

			actions->Start(U"JumpPunch");
		}
		else
		{
			actions->Start(U"Punching");
		}
	}

	double speed_rate = 1.0;

	if (KeyJ.pressed()) { speed_rate = 2.0; };//ダッシュ

	if (not actions->Active(U"Punching"))
	{
		//ジャンプ
		if (KeyW.down() and box.touch(down_line))
		{
			actions->Start(U"Jumping");
		}
		if (KeyA.pressed() and (not KeyD.pressed()) and (not box.touch(left_line)))
		{
			//左
			vel.x = -walk_speed * speed_rate;
			direction.x = -1;
		}
		else if (KeyD.pressed() and (not KeyA.pressed()) and (not box.touch(right_line)))
		{
			//右
			vel.x = walk_speed * speed_rate;
			direction.x = 1;
		}
		else
		{
			vel.x = 0;
		}
	}
	else
	{
		vel.x = 0;
	}

	if (box.touch(up_line) && vel.y < 0)vel.y = 0;//天井に衝突したらy軸速度を0に
	if (box.touch(down_line) && vel.y > 0)vel.y = 0;//床に衝突したらy軸速度を0に
}

void Player::draw2()
{
	
}
