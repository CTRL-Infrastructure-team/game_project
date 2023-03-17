#pragma once
#include"c_timer.h"

class Acts;
class Act;
//開始したactとその開始した時間を記録するリスト
class Sets
{
private:
	HashTable<String, std::pair<double, double>>actSets;
	int32 end_act_num = 0;
public:
	bool isMember(String act_id) { for (auto set : actSets)if (set.first == act_id)return true; return false; };
	//まだ追加されていない要素だったら追加
	void add_start(String act_id, double time) {
		if (not isMember(act_id)) actSets.emplace(act_id, std::pair<double, double>(time, 0)); else actSets[act_id] = std::pair<double, double>{ time,actSets[act_id].second };
	}
	void add_end(String act_id, double time) {
		if (not isMember(act_id)) actSets.emplace(act_id, std::pair<double, double>(0, time)); else actSets[act_id] = std::pair<double, double>{ actSets[act_id].first,time };
		end_act_num++;
	}
	//二つ目のreturn はエラー
	double EndTimeOf(String act_id) { if (actSets.contains(act_id))return actSets[act_id].second; else return 0; }
	double StartTimeOf(String act_id) { if (actSets.contains(act_id))return actSets[act_id].first; else return 0; }
	//要素数を得る
	int32 getSize() { return actSets.size(); }

	int32 EndActNum() { return end_act_num; }
	//要素を消す
	void Clear() { actSets.clear(); end_act_num = 0; }
};
/*
class Reseption
{
public:
	Reseption(bool* SystemUpdate):SystemUpdate(SystemUpdate) {};
	~Reseption() {};

	void Set(String ActionName, String PreActionName, double t1, double t2, String from);

	bool Active(String ActionName)
	{
		if (next_action_start.contains(ActionName))
		{
			if (next_action_start[ActionName])
			{
				return true;
			}
		}
		if (next_action_end.contains(ActionName))
		{
			if (next_action_end[ActionName])
			{
				return true;
			}
		}
		return false;
	}

	void update()
	{
		Print << Started;
		for (const auto& accept : accept_nextAction_start)
		{
			bool flag = false;
			for (const auto& started : Started)
			{
				if (accept.first == started)
				{
					flag = true;
				}
			}
			if (not flag)
			{
				for (const auto& pre : accept.second)
				{
					if (Start_timer.contains(accept.first))
					{
						if (Start_timer[accept.first].contains(pre.first))
						{
							if (Start_timer[accept.first][pre.first]->GetLimitTimerRunning())
							{
								Start_timer[accept.first][pre.first]->update();
								if (pre.second.t1 <= Start_timer[accept.first][pre.first]->time()
									and pre.second.t2 >= Start_timer[accept.first][pre.first]->time())
								{
									next_action_start[accept.first] = true;
								}
								else
								{
									next_action_start[accept.first] = false;
								}
							}
						}
					}
					if (End_timer.contains(accept.first))
					{
						if (End_timer[accept.first].contains(pre.first))
						{
							if (End_timer[accept.first][pre.first]->GetLimitTimerRunning())
							{
								End_timer[accept.first][pre.first]->update();
								if (pre.second.t1 <= End_timer[accept.first][pre.first]->time()
									and pre.second.t2 >= End_timer[accept.first][pre.first]->time())
								{
									next_action_start[accept.first] = true;
								}
								else
								{
									next_action_start[accept.first] = false;
								}
							}

						}
					}
				}
			}
		}
	};

	void ActionStart(String ActionName)
	{
		bool flag = false;
		for (const auto& started : Started)
		{
			if (ActionName == started)
			{
				flag = true;
			}
		}
		if (not flag)Started << ActionName;
		if (next_action_start.contains(ActionName))next_action_start[ActionName] = false;
		if (next_action_end.contains(ActionName))next_action_end[ActionName] = false;
		if (Start_timer.contains(ActionName))
		{
			for (auto& timer : Start_timer[ActionName])
			{
				timer.second->End();
			}
		}
	}

	void ActionEnd(String ActionName)
	{
		for (auto it = Started.begin(); it != Started.end();)
		{
			if (*it == ActionName)it = Started.erase(it);
			else it++;
		}
	}

	void Start_start(String ActionName)
	{
		ActionStart(ActionName);
		for (auto& timer : Start_timer)
		{
			if (timer.second.contains(ActionName)) { timer.second[ActionName]->ReStartLimitTimer(); }
			if (End_timer.contains(ActionName)) { if (End_timer[ActionName].contains(timer.first))End_timer[ActionName][timer.first]->End(); }
		}
	};

	void Start_end(String ActionName)
	{
		ActionEnd(ActionName);
		for (auto& timer : End_timer)
		{
			if (timer.second.contains(ActionName))timer.second[ActionName]->ReStartLimitTimer();
		}
	};
private:
	struct TimeSection
	{
		//t1秒後t2秒以内
		double t1;
		double t2;
	};
	//一つ目のStringは次のアクションの名前
	HashTable<String, HashTable<String, TimeSection>>accept_nextAction_start;
	HashTable<String, HashTable<String, TimeSection>>accept_nextAction_end;
	HashTable<String, HashTable<String,c_timer*>>Start_timer;
	HashTable<String, HashTable<String,c_timer*>>End_timer;
	HashTable<String, bool>next_action_start;
	HashTable<String, bool>next_action_end;
	//実行されたアクションをいれる
	Array<String>Started;
	bool* SystemUpdate;
};*/

class ActionManager
{
public:
	ActionManager(bool* system_update) :System_update(system_update) {}

	~ActionManager() {};

	void ReadJSON(String path);

	void SetAct(String actionName, String actName, Act* act);

	void SetAct(String actionName1, String actionName2, String actName,Act* act);

	void Set_all ();

	void Start(String ActionName);

	void Start(String ActionName1, String ActionName2);

	void update();

	void draw();

	void drawTexture();

	void End(String ActionName);

	void End(String ActionName1, String ActionName2);

	void StartAct(String ActionName, String ActName);

	void StartAct(String ActionName1, String ActionName2, String ActName);

	void EndAct(String ActionName, String ActName);

	void EndAct(String ActionName1, String ActionName2, String ActName);

	bool Active(String ActionName);

	bool Active(String ActionName1, String ActionName2);

	void Show(String ActionName);
	void Show(String ActionName1, String ActionName2);
private:
	struct Conection
	{
		String name;
		bool from;
		double time;
	};
	
	struct c_Act {
		String name;
		double length, start_time;
		bool length_used, start_time_used;
		Array<Conection>Start, Continue;
	};
	//Reseption next_action;
	HashTable<String, Acts*>actions;
	HashTable<String,Array<c_Act>> acts;
	JSON ActionJSON;
	bool* System_update;
};
