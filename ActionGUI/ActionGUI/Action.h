#pragma once
#include"Act.h"
#include"Arrow.h"

namespace action {
	struct Switch;
}

class Action
{
public:
	Action();
	~Action();
	void SetName(String Name);
	/*
	void SetStart(action::Switch* start);
	void SetEnd(action::Switch* end);
	void AddReq(action::Switch* req);*/
	String Get_name() { return name; }
	String* Get_name_pointer() { return &name; }

	void conectionUpdate();

	HashTable<int32,Arrow> acts;//int32 はarrowid
	HashTable<int32, Array<act::Switch>> conections_start;
	HashTable<int32, Array<act::Switch>> conections_end;
	int32 countArrow;
	String name;
private:
/*	Array<action::Switch*> start;//このスイッチがスタートの合図
	Array<action::Switch*> end;//このスイッチが終了の合図
	Array<action::Switch*> req;//アクションを実行できる条件*/
};
