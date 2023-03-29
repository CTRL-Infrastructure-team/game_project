#include "../../stdafx.h"
#include "Attack.h"

Attack::Attack(Figure Hitbox,double* power, Character* Attacker, Array<Character*>* list, CharaType target)
	:hitbox(HitBox{ new Vec2{},new Vec2{},Hitbox }), power(power), attacker(Attacker), list(list), target(target)
{
	FirstRelative = hitbox.Relative;
}

Attack::~Attack() {}

void Attack::startAct()
{
	if(attacker->direction == Vec2{ -1,0 })
	{
		hitbox.Relative.x = -FirstRelative.x;
	}
	else
	{
		hitbox.Relative.x = FirstRelative.x;
	}

	hitbox = HitBox(&attacker->pos, &attacker->vel, hitbox.Get_Box(), hitbox.Relative);
}

void Attack::endAct()
{

}

void Attack::update()
{
	hitbox.update();

	for (auto& chara : *list)
	{
		if (chara->Type() == target and hitbox.intersects(chara->box))
		{
			chara->takeDamage(*power);
		}
	}
}

void Attack::draw()
{
	if(drawhitbox)hitbox.draw(ColorF(Palette::Red,0.7));
}

