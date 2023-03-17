#include"stdafx.h"
#include"HitBox.h"

//二つの値の距離を返す
double myAbsDiff(double x, double y);

HitBox::HitBox(Vec2* Pos, Vec2* vel, RectF box)
	:pos(Pos), PrePos(*Pos), physics(pos, vel, -Vec2{box.w/2,box.h/2}, box.w, box.h)
{
	Relative = box.center();
	hitbox.box.rect = box;
	hitbox.box.rect.setCenter(*pos + Relative);
	hitbox.shape = Shape::rect;

}

HitBox::HitBox(Vec2* Pos, Vec2* vel, Circle box)
	:pos(Pos), PrePos(*Pos), physics(pos, vel, -Vec2{ box.r,box.r }, 2 * box.r, 2 * box.r)
{
	Relative = box.center;
	hitbox.box.circle = box;
	hitbox.box.circle.setCenter(*pos + Relative);
	hitbox.shape = Shape::circle;
}

HitBox::HitBox(Vec2* Pos, Vec2* vel,Triangle box, Vec2 relative)
	:pos(Pos), PrePos(*Pos), Relative(relative),
	physics(pos, vel,
		-Vec2
		{
			Max({ myAbsDiff(box.p0.x, box.p1.x) ,myAbsDiff(box.p0.x, box.p2.x),myAbsDiff(box.p2.x, box.p1.x) }) / 2,
			Max({myAbsDiff(box.p0.y, box.p1.y) ,myAbsDiff(box.p0.y, box.p2.y),myAbsDiff(box.p1.y, box.p2.y)}) / 2
		},
		Max({ myAbsDiff(box.p0.x, box.p1.x) ,myAbsDiff(box.p0.x, box.p2.x),myAbsDiff(box.p2.x, box.p1.x) }),
		Max({ myAbsDiff(box.p0.y, box.p1.y) ,myAbsDiff(box.p0.y, box.p2.y),myAbsDiff(box.p2.y, box.p1.y) }))
{
	hitbox.box.triangle = box.movedBy(*pos + Relative);
	hitbox.shape = Shape::triangle;
}

HitBox::HitBox(Vec2* Pos, Vec2* vel, box Hitbox, Vec2 relative)
	:pos(Pos), PrePos(*Pos), Relative(relative), physics(NULL, NULL, Vec2{ 0,0 },0,0)
{
	switch (Hitbox.shape)
	{
	case Shape::rect:
		physics=PhysicsBox{ pos, vel, -Vec2{ Hitbox.box.rect.w / 2,Hitbox.box.rect.h / 2 }, Hitbox.box.rect.w, Hitbox.box.rect.h };
		hitbox.box.rect = Hitbox.box.rect;
		hitbox.box.rect.setCenter(*pos + Relative);
		hitbox.shape = Shape::rect;
		break;
	case Shape::circle:
		physics = PhysicsBox(pos, vel, -Vec2{ Hitbox.box.circle.r,Hitbox.box.circle.r }, 2 * Hitbox.box.circle.r, 2 * Hitbox.box.circle.r);
		hitbox.box.circle = Hitbox.box.circle;
		hitbox.box.circle.setCenter(*pos + Relative);
		hitbox.shape = Shape::circle;
		break;
	case Shape::triangle:
		physics = PhysicsBox(pos, vel,
		-Vec2
		{
			Max({ myAbsDiff(Hitbox.box.triangle.p0.x, Hitbox.box.triangle.p1.x) ,myAbsDiff(Hitbox.box.triangle.p0.x, Hitbox.box.triangle.p2.x),myAbsDiff(Hitbox.box.triangle.p2.x, Hitbox.box.triangle.p1.x) }) / 2,
			Max({myAbsDiff(Hitbox.box.triangle.p0.y, Hitbox.box.triangle.p1.y) ,myAbsDiff(Hitbox.box.triangle.p0.y, Hitbox.box.triangle.p2.y),myAbsDiff(Hitbox.box.triangle.p1.y, Hitbox.box.triangle.p2.y)}) / 2
		},
		Max({ myAbsDiff(Hitbox.box.triangle.p0.x, Hitbox.box.triangle.p1.x) ,myAbsDiff(Hitbox.box.triangle.p0.x, Hitbox.box.triangle.p2.x),myAbsDiff(Hitbox.box.triangle.p2.x, Hitbox.box.triangle.p1.x) }),
		Max({ myAbsDiff(Hitbox.box.triangle.p0.y, Hitbox.box.triangle.p1.y) ,myAbsDiff(Hitbox.box.triangle.p0.y, Hitbox.box.triangle.p2.y),myAbsDiff(Hitbox.box.triangle.p2.y, Hitbox.box.triangle.p1.y) }));
		hitbox.box.triangle = Hitbox.box.triangle.movedBy(*pos + Relative);
		hitbox.shape = Shape::triangle;
		break;
	}
}

bool HitBox::intersects(HitBox target)
{
	switch (hitbox.shape)
	{
	case rect:
		switch (target.Get_Box().shape)
		{
		case rect:
			return hitbox.box.rect.intersects(target.Get_Box().box.rect);
		case circle:
			return hitbox.box.rect.intersects(target.Get_Box().box.circle);
		case triangle:
			return hitbox.box.rect.intersects(target.Get_Box().box.triangle);
		default:
			return false;
		}
	case circle:
		switch (target.Get_Box().shape)
		{
		case rect:
			return hitbox.box.circle.intersects(target.Get_Box().box.rect);
		case circle:
			return hitbox.box.circle.intersects(target.Get_Box().box.circle);
		case triangle:
			return hitbox.box.circle.intersects(target.Get_Box().box.triangle);
		default:
			return false;
		}
	case triangle:
		switch (target.Get_Box().shape)
		{
		case rect:
			return hitbox.box.triangle.intersects(target.Get_Box().box.rect);
		case circle:
			return hitbox.box.triangle.intersects(target.Get_Box().box.circle);
		case triangle:
			return hitbox.box.triangle.intersects(target.Get_Box().box.triangle);
		default:
			return false;
		}
	default:
		return false;
	}
}

void HitBox::SetShape(RectF box)
{
	Relative = box.center();
	hitbox.box.rect = box;
	hitbox.box.rect.setCenter(*pos + Relative);
	hitbox.shape = Shape::rect;
	physics.delta = -Vec2{ box.w / 2,box.h / 2 };
	physics.width = box.w; physics.height = box.h;
}

void HitBox::SetShape(Circle box)
{
	Relative = box.center;
	hitbox.box.circle = box;
	hitbox.box.circle.setCenter(*pos + Relative);
	hitbox.shape = Shape::circle;
	physics.delta = -Vec2{ box.r,box.r };
	physics.width = physics.height = 2 * box.r;
}

void HitBox::SetShape(Triangle box,Vec2 relative)
{

}

void HitBox::setPos(Vec2 position)
{
	switch (hitbox.shape)
	{
	case rect:
		hitbox.box.rect.setCenter(position);
		break;
	case circle:
		hitbox.box.circle.setCenter(position);
		break;
	case triangle:
		//hitbox.box.triangleまだ定義しない
		break;
	default:
		break;
	}
}

void HitBox::update()
{
	switch (hitbox.shape)
	{
	case rect:
		hitbox.box.rect = hitbox.box.rect.movedBy(*pos - PrePos);
		break;
	case circle:
		hitbox.box.circle = hitbox.box.circle.movedBy(*pos - PrePos);
		break;
	case triangle:
		hitbox.box.triangle = hitbox.box.triangle.movedBy(*pos - PrePos);
		break;
	default:
		break;
	}
	PrePos = *pos;
}

void HitBox::draw(ColorF color)
{
	switch (hitbox.shape)
	{
	case rect:
		hitbox.box.rect.draw(color);
		break;
	case circle:
		hitbox.box.circle.draw(color);
		break;
	case triangle:
		hitbox.box.triangle.draw(color);
		break;
	default:
		break;
	}
}

double myAbsDiff(double x, double y)
{
	if (x - y < 0)return y - x;
	return x - y;
}
