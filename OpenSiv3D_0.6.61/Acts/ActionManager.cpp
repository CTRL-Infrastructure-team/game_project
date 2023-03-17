#include "../stdafx.h"
#include "ActionManager.h"
#include"Acts.h"
#include"Act/Switch.h"

void ActionManager::ReadJSON(String path)
{
	JSON json = JSON::Load(path);
	if (not json)throw Error{ U"ファイルを読み込めませんでした。" };
	for (const auto& action : json[U"Actions"].arrayView())
	{
		Array<c_Act>model;
		String acts_name = action[U"name"].get<String>();
		for (const auto& act : action[U"Acts"].arrayView())
		{
			c_Act l_act;
			l_act.name = act[U"name"].get<String>();
			act[U"Continue"];
			if (act[U"Continue"][U"length"].get<double>() == -1)l_act.length_used = false; else l_act.length_used = true;
			l_act.length = act[U"Continue"][U"length"].get<double>();
			for (const auto& conection : act[U"Continue"][U"Conection"].arrayView())
			{
				String name = conection[U"name"].get<String>();
				bool from = (conection[U"from"].get<String>() == U"start");
				double time = conection[U"time"].get<double>();
				l_act.Continue << Conection{ name, from, time };
			}
			if (act[U"Start"][U"start_time"].get<double>() == -1)l_act.start_time_used = false; else l_act.start_time_used = true;
			l_act.start_time = act[U"Start"][U"start_time"].get<double>();
			for (const auto& conection : act[U"Start"][U"Conection"].arrayView())
			{
				String name = conection[U"name"].get<String>();
				bool from = (conection[U"from"].get<String>() == U"start");
				double time = conection[U"time"].get<double>();
				l_act.Start << Conection{ name, from, time };
			}
			model << l_act;
		}
		acts.emplace(acts_name, model);
		actions.emplace(acts_name, new Acts{ System_update });
	}
	ActionJSON = json;
}

void ActionManager::SetAct(String actionName, String actName, Act* act)
{
	if (not acts.contains(actionName))throw Error{ U"action「" + actionName + U"」はありません。" };
	bool flag = false;
	c_Act c_act;
	for (auto& act : acts[actionName])
	{
		if (act.name == actName)
		{
			flag = true;
			c_act = act;
		}
	}
	if (not flag)throw Error{ U"action「"+actionName+U"」にact「" + actName + U"」はありません。"};
	actions[actionName]->SetAct(act, actName);
	if (c_act.length_used)actions[actionName]->AddLength(c_act.length, actName);
	if (c_act.start_time_used)actions[actionName]->AddStart(c_act.start_time, actName);
}

void ActionManager::SetAct(String actionName1, String actionName2, String actName, Act* act)
{
	if (not acts.contains(actionName1))throw Error{ U"action「" + actionName1 + U"」はありません。" };
	bool flag = false;
	c_Act c_act;
	for (auto& act : acts[actionName1])
	{
		if (act.name == actName)
		{
			flag = true;
			c_act = act;
		}
	}
	if (not flag)throw Error{ U"action「" + actionName1 + U"」にact「" + actName + U"」はありません。" };

	String actionName = actionName1 + U"/" + actionName2;
	Array<c_Act> ac = acts[actionName1];
	if (not acts.contains(actionName)) acts.emplace(actionName, ac);
	if (not actions.contains(actionName))actions.emplace(actionName, new Acts{ System_update });
	
	actions[actionName]->SetAct(act, actName);
	if (c_act.length_used)actions[actionName]->AddLength(c_act.length, actName);
	if (c_act.start_time_used)actions[actionName]->AddStart(c_act.start_time, actName);
}

void ActionManager::Set_all()
{
	for (auto& action : acts)
	{
		actions[action.first]->SetSwitch();
		for (auto& act : action.second)
		{
			for (auto& conection : act.Start)
			{
				actions[action.first]->Add(conection.name, conection.time, conection.from, true, act.name);
			}
			for (auto& conection : act.Continue)
			{
				actions[action.first]->Add(conection.name, conection.time, conection.from, false, act.name);
			}
		}
	}
}

void ActionManager::Start(String ActionName)
{
	if (actions.contains(ActionName))
	{
		actions[ActionName]->Start();
	}
}

void ActionManager::Start(String ActionName1, String ActionName2)
{
	String ActionName = ActionName1 + U"/" + ActionName2;

	if (actions.contains(ActionName))
	{
		actions[ActionName]->Start();
	}
}

void ActionManager::update()
{
	for (auto& action : actions)
	{
		action.second->update();
		if (action.second->end)
		{
			action.second->End();
		}
	}
}

void ActionManager::draw()
{
	for (const auto& action : actions)
	{
		action.second->draw();
	}
}

void ActionManager::drawTexture()
{
	for (const auto& action : actions)
	{
		action.second->drawTexture();
	}
}

void ActionManager::End(String ActionName)
{
	if (actions.contains(ActionName))
	{
		actions[ActionName]->End();
	}
}

void ActionManager::End(String ActionName1, String ActionName2)
{
	String ActionName = ActionName1 + U"/" + ActionName2;
	if (actions.contains(ActionName))
	{
		actions[ActionName]->End();
	}
}

void ActionManager::StartAct(String ActionName, String ActName)
{
	if(actions.contains(ActionName))actions[ActionName]->StartAct(ActName);
}

void ActionManager::StartAct(String ActionName1, String ActionName2, String ActName)
{
	String ActionName = ActionName1 + U"/" + ActionName2;
	if (actions.contains(ActionName))actions[ActionName]->StartAct(ActName);
}

void ActionManager::EndAct(String ActionName, String ActName)
{
	if(actions.contains(ActionName))actions[ActionName]->EndAct(ActName);
}

void ActionManager::EndAct(String ActionName1, String ActionName2, String ActName)
{
	String ActionName = ActionName1 + U"/" + ActionName2;
	if (actions.contains(ActionName))actions[ActionName]->EndAct(ActName);
}

bool ActionManager::Active(String ActionName)
{
	if (actions.contains(ActionName))return actions[ActionName]->Working;
	return false;
}

bool ActionManager::Active(String ActionName1, String ActionName2)
{
	String ActionName = ActionName1 + U"/" + ActionName2;
	if (actions.contains(ActionName))return actions[ActionName]->Working;
	return false;
}

void ActionManager::Show(String ActionName)
{
	actions[ActionName]->Show();
}

void ActionManager::Show(String ActionName1, String ActionName2)
{
	String ActionName = ActionName1 + U"/" + ActionName2;
	Show(ActionName);
}
