#pragma once
#include "Act/Act.h"
#include"ActionManager.h"
#include"Act/Switch.h"

struct ActsTrigger {
	String act_name;
	bool from;//trueならTrigger開始からtime秒後、falseなら終了からtime秒後
	double time;
	bool trigger_type;//trueなら開始条件、endなら続行条件
};

class Acts
{
public:
	Acts(bool* System_update):System_update_id(System_update)
	{
		timer = new c_timer(System_update_id);
	}

	void SetSwitch()
	{
		for (auto& act : acts)
		{
			Switch start_s, end_s; start_s.Set_timer(System_update_id); end_s.Set_timer(System_update_id);
			start_switch.emplace(act.first, start_s);
			end_switch.emplace(act.first, end_s);
		}
	}

	void SetStart(double start_time, Act* act, String Name)
	{
		act->Set_timer(System_update_id);
		acts.emplace(Name, ACT{ Array<ActsTrigger>{},true,start_time,false,0,act });
		numAct++;
	}
	void SetLength(double length, Act* act, String Name)
	{
		act->Set_timer(System_update_id);
		acts.emplace(Name, ACT{ Array<ActsTrigger>{},false,0,true,length,act });
		numAct++;
	}

	void SetAct(Act* act, String Name)
	{
		act->Set_timer(System_update_id);
		acts.emplace(Name, ACT{ Array<ActsTrigger>{},false,0,false,0,act });
		numAct++;
	}

	//開始時間と長さ
	void SetAct(double start_time, double length, Act* act, String Name)
	{
		act->Set_timer(System_update_id);
		acts.emplace(Name, ACT{ Array<ActsTrigger>{},true,start_time,true,length,act });
		numAct++;
	}
	//fromがtrueならtrigger開始からtime秒後
	void SetAct(String trigger_act_name, double time, bool from, Act* act, String Name)
	{
		act->Set_timer(System_update_id);
		acts.emplace(Name, ACT{ Array<ActsTrigger>{ActsTrigger{trigger_act_name,from,time}},false,0,false,0,act });
		numAct++;
	}
	//条件の追加
	void AddStart(double start_time, String Name)
	{
		acts[Name].start_time_use = true;
		acts[Name].start_time = start_time;
	}

	void AddLength(double length, String Name)
	{
		acts[Name].length_use = true;
		acts[Name].length = length;
	}

	void Add(double start_time, double length, String Name)
	{
		acts[Name].start_time_use = true;
		acts[Name].start_time = start_time;
		acts[Name].length_use = true;
		acts[Name].length = length;
	}

	void Add(String trigger_act_name, double time, bool from,bool trigger_type, String Name)
	{
		acts[Name].Trigger << ActsTrigger{ trigger_act_name,from,time,trigger_type };
	}

	void Start();

	void update();

	void draw();

	void drawTexture();

	void End();

	void StartAct(String ActName);
	void EndAct(String ActName);

	void SetNextAction(String ActionName, double t1, double t2);

	bool NextAction(String Name);

	void Show()
	{
		for (auto& act : acts)
		{
			Print << U"メインアクトのアドレス、length、start_time";
			Print << act.second.mainAct;
			Print << act.second.length_use << act.second.length;
			Print << act.second.start_time_use << act.second.start_time;
			Print << U"トリガー";
			for (auto& tri : act.second.Trigger)
			{
				Print << tri.act_name;
				Print << tri.from;
				Print << tri.time;
				Print << tri.trigger_type;
			}
		}
	};

	bool Working = false;

	bool end = false;//これがtrueになったとき、ActionManagerがActsを終了させる(Endを呼び出す)

private:
	String name;

	struct ACT
	{
		Array<ActsTrigger> Trigger;
		bool start_time_use;
		double start_time;
		bool length_use;
		double length;
		Act* mainAct;
	};
	//のちのち名前を指定して条件付けを行いそうなのでハッシュ
	//~Actが実行してなければみたいな条件
	HashTable<String, ACT> acts;
	HashTable<String, Switch>start_switch;
	HashTable<String, Switch>end_switch;

	c_timer *timer;

	int32 numAct=0;

	bool* System_update_id;

	Sets actSets;
};

