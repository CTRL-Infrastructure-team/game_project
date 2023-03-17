#pragma once


class Ground :public Object
{
public:
	Ground() { name = { U"Ground" }; }
	void reaction(Point pos, PhysicsBox* box) {
		box->hantei(Rect{ pos * rect_size,rect_size });
	}
	void draw(Point pos) { Rect{ pos * rect_size,rect_size }.draw(Palette::Green); }
};

class Wall :public Object
{
public:
	Wall() { name = { U"Wall" }; }
	void reaction(Point pos, PhysicsBox* box) {
		box->hantei(Rect{ pos * rect_size,rect_size });
	}
	void draw(Point pos) { Rect{ pos * rect_size,rect_size }.draw(Palette::Saddlebrown); }
};

class Ball :public Object
{
public:
	Ball() { name = { U"Ball" }; }
	void reaction(Point pos, PhysicsBox* box) {
		box->hantei(Circle{ pos * rect_size,rect_size });
	}
	void draw(Point pos) { Circle{ pos * rect_size,rect_size }.draw(Palette::Gray); }
};

class Slider :public Object
{
public:
	Slider() { name = { U"Slider" }; }
	void reaction(Point pos, PhysicsBox* box) {
		pos *= rect_size;
		Triangle triangle{ pos, pos + Point(0,rect_size), pos + Point(rect_size,rect_size) };
		box->hantei(triangle);
		if (box->lines(left_line).intersects(triangle)) {
			*(box->pos)+=Vec2(5,5);
		}
	}
	void draw(Point pos) {
		pos *= rect_size;
		Triangle{ pos, pos + Point(0,rect_size), pos + Point(rect_size,rect_size) }.draw(Palette::Orange);
	}
};

class Moving :public Object
{
public:
	Moving() { name = { U"Moving" }; }
	void reaction(Point pos, PhysicsBox* box) {
		RectF rect{ pos * rect_size,rect_size };
		rect.x += Periodic::Triangle0_1(3s) * rect_size * 3;
		if (box->lines(down_line).intersects(rect)) {
			if (Periodic::Square0_1(3s))box->pos->x += Scene::DeltaTime()/1.5 * rect_size * 3;
			else box->pos->x -= Scene::DeltaTime()/1.5 * rect_size * 3;
		}
		box->hantei(rect);
	}
	void draw(Point pos) {
		RectF rect{ pos * rect_size,rect_size };
		rect.x += Periodic::Triangle0_1(3s) * rect_size * 3;
		rect.draw(Palette::Black);
	}
};

class Elevator :public Object
{
public:
	Elevator() { name = { U"Elevator" }; }
	void reaction(Point pos, PhysicsBox* box) {
		Rect rect{ pos * rect_size,rect_size };
		rect.y += rect_size * 3;
		rect.y -= Periodic::Triangle0_1(3s) * rect_size * 5;
		box->hantei(rect);
	}
	void draw(Point pos) {
		Rect rect{ pos * rect_size,rect_size };
		rect.y += rect_size * 3;
		rect.y -= Periodic::Triangle0_1(3s) * rect_size * 5;
		rect.draw(Palette::Yellow);
	}
};
class Spring :public Object
{
public:
	Spring() { name = { U"Spring" }; }
	void reaction(Point pos, PhysicsBox* box) {
		Rect rect{ pos * rect_size,rect_size };
		box->hantei(rect);
		if (box->lines(down_line).intersects(rect))(box->vel->y) = -Min(Abs(box->vel->y),800.0);
	}
	void draw(Point pos) { Rect{ pos * rect_size,rect_size }.draw(Palette::Purple); }
};

Array<Object*>objects{ new Wall(),new Ground(),new Ball(),new Slider(),new Moving(),new Elevator(),new Spring() };

