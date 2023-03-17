#pragma once
#include"Act.h"

class Arrow {
public:
	Arrow() { mode = nomal; };
	~Arrow() {};

	void SetArrow(Point start, Point end, int32 arrow_num,ColorF Color,ColorF Uniq) {
		Start = start;
		line.set(start, end);
		act.SetName(U"act"+Format(arrow_num));
		UniqColor = Uniq;
		nomalColor = Color;
	}

	void Mouse_over() {
		mode = mouse_over;
	}
	void Nomal() {
		mode = nomal;
	}
	void Select()
	{
		mode = select;
	}
	Line GetLine() { return line; }
	Point GetStartPos() { return Start;}

	void ShiftStart_to(Point pos)
	{
		line.end += -line.begin + pos;
		line.begin = pos;
		Start = pos;
	}

	void draw() {
		if (mode == select or mode==mouse_over) {
			line.drawArrow(5, Vec2{ 10,10 }, UniqColor);
			Circle{ GetStartPos(),6 }.draw(UniqColor);
		}
		else
		{
			line.drawArrow(5, Vec2{ 10,10 }, nomalColor);
			Circle{ GetStartPos(),6 }.draw(nomalColor);
		}
	}

	ColorF UniqColor;
	ColorF nomalColor;
	Act act;
	enum Mode {
		nomal,
		mouse_over,
		select,
		chosen
	} mode;
private:
	Line line;
	Point Start;
};
