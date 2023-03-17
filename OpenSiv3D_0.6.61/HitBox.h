#pragma once
#include"Object.h"
constexpr int rect_size = 70;

union Figure{
	RectF rect;
	Circle circle;
	Triangle triangle;
};
enum Shape {
	rect,
	circle,
	triangle,
};
struct box {
	Figure box;
	Shape shape;
};

enum direction { up_line, down_line, right_line, left_line };

//ステージとの当たり判定に必要なクラス（このクラスをHitBoxに持たせる）
class PhysicsBox {
public:
	Vec2* pos, * vel;
	bool touch_flag[4] = { 0 };
	double width, height;
	Vec2 delta;//座表と左上の座標との差

	PhysicsBox(Vec2* pos, Vec2* vel, Vec2 delta, double width, double height) :pos(pos), vel(vel), delta(delta), width(width), height(height) {}

	void stage(Grid<Object*>* map) {
		touch_flag[0] = touch_flag[1] = touch_flag[2] = touch_flag[3] = 0;
		int range = 6;
		Point point = pos->asPoint() / Point(rect_size, rect_size);
		for (int y = Max(0, point.y - range); y < Min((int)map->height(), point.y + range); y++)
		{
			for (int x = Max(0, point.x - range); x < Min((int)map->width(), point.x + range); x++) {
				Point p(x, y);
				if ((*map)[p] != NULL)(*map)[p]->reaction(p, this);//reaction関数がhanteiに図形を渡して呼び出す
			}
		}
	}
	//それぞれの方向のshift関数を呼び出す
	template<typename T>void hantei(T rect) {
		shift(rect, up_line, Vec2{ 0,0.1 });
		shift(rect, down_line, Vec2{ 0,-0.1 });
		shift(rect, right_line, Vec2{ -0.1,0 });
		shift(rect, left_line, Vec2{ 0.1,0 });
	}

	//図形の埋め込みを解消する
	template<typename T>void shift(T rect, direction num, Vec2 vec)
	{
		if (not rect.intersects(lines(num)))return;//Lineと当たっていなければ何もしない
		while (rect.intersects(lines(num)))*pos += vec;//Lineと当たらなくなるまで移動
		*pos -= vec;//少し戻す（これをしないと、ブロックと当たる、離れるを繰り返してしまう。）
		touch_flag[num] = true;//その方向の
	}

	//それぞれの方向のLineを返す
	Line lines(direction num)
	{
		Vec2 p0 = *pos + delta;
		double d = 10;
		switch (num) {
		case up_line:return 	Line(p0 + Vec2(0 + d, 0), p0 + Vec2(width - d, 0));
		case down_line:return 	Line(p0.x + d, p0.y + height, p0.x + width - d, p0.y + height);
		case right_line:return 	Line(p0 + Vec2(width, 0 + d), p0 + Vec2(width, height - d));
		case left_line:return 	Line(p0 + Vec2(0, 0 + d), p0 + Vec2(0, height - d));
		}
	}
};

class HitBox
{
public:
	HitBox(Vec2* pos, Vec2* vel, RectF box);
	HitBox(Vec2* pos, Vec2* vel, Circle box);
	HitBox(Vec2* pos, Vec2* vel, Triangle box, Vec2 Relative = Vec2{0,0});
	HitBox(Vec2* pos, Vec2* vel, box hitbox, Vec2 Relative = Vec2{ 0,0 });

	virtual ~HitBox(){}

	bool intersects(HitBox target);

	void update();

	void draw(ColorF color);

	void setPos(Vec2 position);

	box Get_Box() { return hitbox; }

	void SetShape(RectF box);
	void SetShape(Circle box);
	void SetShape(Triangle box,Vec2 relative);

	bool touch(direction num) { return physics.touch_flag[num]; }
	//ステージから影響を受ける
	void stage(Grid<Object*>* map) { physics.stage(map); }

	Vec2* pos, PrePos, Relative;
	PhysicsBox  physics;
protected:
	box hitbox;
};
