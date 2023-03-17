#pragma once
#include"../HitBox.h"
#include"../Acts/ActionManager.h"

constexpr Vec2 Right{ 1,0 };
constexpr Vec2 Left{ -1,0 };

enum CharaType
{
	player,
	enemy,
};

class Character
{
public:
	Character(Vec2 init_pos, Vec2 init_vel, Vec2 init_acc, CharaType chara, HitBox hitbox, Array<Character*>* List, bool* systemUpdate)
		:pos(init_pos), vel(init_vel), acc(init_acc), type(chara), box(hitbox), list(List)
	{
		*list << this;
		actions = new ActionManager(systemUpdate);
		SetHitBoxColor(ColorF(Palette::Blue, 0.6));
	};
	virtual ~Character() {};

	void Set_G(double* G) { acc.y = *G; g = G; }

	double Get_G() { return *g; }

	virtual void takeDamage(double power) { Print << U"hit"; };

	virtual void update() = 0;

	void ActionsUpdate() { if (actions != NULL) { actions->update(); } }

	void SetHitBoxColor(ColorF Color) { color = Color; }

	void draw_nonvirtual()
	{
		if (actions != NULL)
		{
			actions->drawTexture();
			actions->draw();
		}
		box.draw(color);
		draw();
	}

	virtual void draw()
	{

	}

	void move()
	{
		vel += acc * Scene::DeltaTime();
		pos += vel * Scene::DeltaTime();
	};

	CharaType Type() { return type; }

	Vec2 pos;
	Vec2 vel;
	Vec2 acc;
	HitBox box;
	Vec2 direction{ 1,0 };
	ActionManager* actions = NULL;
protected:
	double* g;//ステージの重力加速度
	CharaType type;
	double hp;
	double power;
	Array<Character*>* list;
	ColorF color;
	double accumulate;
	double dt;
};

