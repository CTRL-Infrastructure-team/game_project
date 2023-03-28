#include"stdafx.h"
#include"HitBox.h"

HitBox::HitBox(Vec2* Pos, Vec2* vel, Figure Hitbox, Vec2 relative)
	:hitbox(Hitbox),pos(Pos), PrePos(*Pos), Relative(relative), physics(NULL, NULL, Vec2{ 0,0 },0,0)
{
	const RectF rect = hitbox.boundingRect();
	hitbox.setCenter(*pos + Relative + (hitbox.center()- rect.center()));
	physics = PhysicsBox{ pos, vel, -Vec2{ rect.w / 2,rect.h / 2 }, rect.w, rect.h };
}

bool HitBox::intersects(HitBox target)
{
	return target.hitbox.intersects(hitbox);
}

void HitBox::SetShape(Figure box, Vec2 relative)
{
	Relative = relative;
	hitbox = box;
	const RectF rect = hitbox.boundingRect();
	hitbox.setCenter(*pos + Relative+ (hitbox.center() - rect.center()));
	physics.delta = -Vec2{ rect.w / 2,rect.h / 2 };
	physics.width = rect.w; physics.height = rect.h;
}

void HitBox::setPos(Vec2 position)
{
	hitbox.setCenter(position);
}

void HitBox::update()
{
	hitbox.moveBy(*pos - PrePos);
	PrePos = *pos;
}

void HitBox::draw(ColorF color)
{
	hitbox.draw(color);
}

