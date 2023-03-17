#pragma once
#include"../c_timer.h"

class Act
{
public:
	Act() {
		started = false; end = false; end1 = false;
	}
	virtual ~Act() {}

	void Set_timer(bool* System_update) { timer = new c_timer(System_update); SystemUpdate = System_update; }

	void Reset() { started = false; end = false; delete timer; timer = timer = new c_timer(SystemUpdate); }
	double get_time() { return timer->time(); }
	void Start(double t = 0);
	virtual void startAct() {};//開始処理
	virtual void endAct() {};//終了処理
	void End();//終了させる
	virtual void update() {};
	void unUse_endact();
	void startAct1() { started = true; end = false; }
	void endAct1() { started = true; end = true; end1 = false; }
	void update1();
	virtual void draw() {};
	bool started;
	bool end;
	bool this_Act_is_TextureDraw = false;
	bool end1;
protected:
	bool endact = true;
	c_timer* timer;
	bool* SystemUpdate;
};

