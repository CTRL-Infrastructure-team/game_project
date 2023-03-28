#include "stdafx.h"
#include "Display.h"
#include"Switch.h"

InputTimeWindow::InputTimeWindow()
{
	close = Rect{ pos + Point{w / 2 - 20,-h / 2},20,17 };
	tex.text = U"0";
	time = 0;
}

InputTimeWindow::~InputTimeWindow() {}

void InputTimeWindow::SetExplanation(String exp)
{
	expla = exp;
}

void InputTimeWindow::SetTime(double t)
{
	tex.text = Format(t);
}

double InputTimeWindow::GetTime()
{
	return time;
}

bool InputTimeWindow::draw()
{
	Rect{ Arg::center(pos),w,h }.draw(Palette::White);

	close.draw(Palette::Red);
	
	Shape2D::Cross(8, 2, close.center()).draw(Palette::White);

	font(expla).drawAt(pos + Vec2{ 0,-h / 4 }, Palette::Black);

	SimpleGUI::TextBoxAt(tex, pos,100);

	if (not tex.active and tex.text.isEmpty())tex.text = U"0";
	
	if (MouseL.down() and close.mouseOver())
	{
		time = Parse<double>(tex.text);
		return true;
	}

	return false;
}

void ExplaWindow::draw()
{
	Rect{ pos,width,height }.draw(color);
	f_title(title,U":").draw(pos, Palette::Black);
	if (not SimpleGUI::TextBox(name, Vec2(pos.x + 30, pos.y + fontsize_Expla_title + 5), TextBoxSize))
		if (p_name != NULL) *p_name = name.text;
}

void InfoWindow::draw()
{
	int32 k = 0;
	Point StartPos{ Scene::Width() - width,0 };
	for (auto& act : action->acts)
	{
		if (act.second.mode == Arrow::select)
		{
			if (act.second.act.start_time_isUsed())
			{
				Rect rect{ StartPos,width,(f_info.fontSize() + 5) * 2 };
				if (rect.mouseOver()) rect.draw(ColorF(Palette::Black, 0.7)); else rect.draw(color);
				f_info(U"・アクション開始から" + Format(act.second.act.Get_start_time()) + U"秒後に開始").draw(rect);
				if (rect.mouseOver() and MouseR.down())act.second.act.ReSetStartTime();
				k++;
			}
			if (act.second.act.length_isUsed())
			{
				Rect rect{ StartPos.x,StartPos.y + k * (f_info.fontSize() + 5) * 2,width,(f_info.fontSize() + 5) * 2 };
				if (rect.mouseOver()) rect.draw(ColorF(Palette::Black, 0.7)); else rect.draw(color);
				f_info(U"・" + Format(act.second.act.Get_length()) + U"秒間続ける").draw(rect);
				if (rect.mouseOver() and MouseR.down())act.second.act.ReSetLengthTime();
				k++;
			}
			if (action->conections_end.contains(act.first))
			{
				for (auto swi = action->conections_end[act.first].begin(); swi != action->conections_end[act.first].end();)
				{
					Rect rect{ StartPos.x,StartPos.y + k * (f_info.fontSize() + 5) * 2,width,(f_info.fontSize() + 5) * 2 };
					if (rect.mouseOver()) rect.draw(ColorF(Palette::Black, 0.7)); else rect.draw(color);
					if (swi->point)
						f_info(U"・「" + *action->acts[swi->arrow_id].act.Get_name_pointer() + U"」" + U"を開始してから" + Format(swi->t) + U"秒後に終了").draw(rect);
					else
						f_info(U"・「" + *action->acts[swi->arrow_id].act.Get_name_pointer() + U"」" + U"が終了してから" + Format(swi->t) + U"秒後に終了").draw(rect);
					if (rect.mouseOver() and MouseR.down())swi = action->conections_end[act.first].erase(swi); else swi++;
					k++;
				}
			}
			if (action->conections_start.contains(act.first))
			{
				for (auto swi = action->conections_start[act.first].begin(); swi != action->conections_start[act.first].end();)
				{
					Rect rect{ StartPos.x,StartPos.y + k * (f_info.fontSize() + 5) * 2,width,(f_info.fontSize() + 5) * 2 };
					if (rect.mouseOver()) rect.draw(ColorF(Palette::Black, 0.7)); else rect.draw(color);
					if (swi->point)
						f_info(U"・「" + *action->acts[swi->arrow_id].act.Get_name_pointer() + U"」" + U"を開始してから" + Format(swi->t) + U"秒後に開始").draw(rect);
					else
						f_info(U"・「" + *action->acts[swi->arrow_id].act.Get_name_pointer() + U"」" + U"が終了してから" + Format(swi->t) + U"秒後に開始").draw(rect);
					if (rect.mouseOver() and MouseR.down())swi = action->conections_start[act.first].erase(swi); else swi++;
					k++;
				}
			}
			
		}
	}
}

ActionSelect::ActionSelect(int32 width, int32 height,Point pos)
{
	window = Rect{ pos,width,height };
	inter = 10;
	a_w = window.w - 20;
	a_h = 30;
	b_w = 30;
	b_h = 20;
	nowPage = lastPage = 0;
	Limit = (int32)(window.h - b_h - inter) / (a_h + inter);
}

ActionSelect::~ActionSelect() {}

void ActionSelect::update()
{
	Triangle tr{ Vec2(window.pos.x + window.w / 2 + 20,window.h + window.pos.y - inter - b_h),
		Vec2(window.pos.x + window.w / 2 + 20,window.h + window.pos.y - inter) ,
	Vec2(window.pos.x + window.w / 2 + 20 + b_w,window.h + window.pos.y - inter - b_h / 2) };

	Triangle tl{ Vec2(window.pos.x + window.w / 2 - 20,window.h + window.pos.y - inter - b_h),
		Vec2(window.pos.x + window.w / 2 - 20,window.h + window.pos.y - inter) ,
	Vec2(window.pos.x + window.w / 2 - 20 - b_w,window.h + window.pos.y - inter - b_h / 2) };

	if (MouseL.down())
	{
		if(tr.mouseOver() and names.contains(nowPage + 1)) nowPage++;
		if(tl.mouseOver() and names.contains(nowPage - 1)) nowPage--;
	}
	//ページをつめる
	HashTable < int32, Array<std::pair<int32, String>>> next_names;
	int32 page1 = 0;
	for (auto page = names.begin(); page != names.end();)
	{
		for (auto name = page->second.begin(); name != page->second.end();)
		{
			if(not next_names.contains(page1))next_names.emplace(page1, Array<std::pair<int32, String>>{});
			next_names[page1] << std::pair<int32, String>{name->first, name->second};
			if (next_names[page1].size() == Limit)page1++;
			name++;
		}
		page++;
	}
	names.clear();
	names = next_names;
}

int32 ActionSelect::Erase()
{
	int32 h{ window.pos.y };
	int32 count = 0;
	while (Limit > count and count < names[nowPage].size())
	{
		h += inter;
		Rect rect{ window.x + 10, h, a_w, a_h };
		if (rect.mouseOver() and MouseR.down())return names[nowPage][count].first;
		count++;
		h += a_h;
	}
	return -1;
}

int32 ActionSelect::Select()
{
	int32 h{ window.pos.y };
	int32 count = 0;
	while (Limit > count and count < names[nowPage].size())
	{
		h += inter;
		Rect rect{ window.x + 10, h, a_w, a_h };
		if (rect.mouseOver() and MouseL.down())return names[nowPage][count].first;
		count++;
		h += a_h;
	}
	return -1;
}

void ActionSelect::NamesUpdate(HashTable<int32, Action> actions)
{
	for (auto& page : names)
	{
		for (auto name = page.second.begin();name!=page.second.end();)
		{
			if (actions.contains(name->first))
			{
				name->second = actions[name->first].Get_name();
				name++;
			}
			else
			{
				name = page.second.erase(name);
			}
		}
	}
}

void ActionSelect::draw()
{
	window.draw(ColorF(Palette::Black, 0.8));
	int32 h{ window.pos.y };
	int32 count = 0;
	while (Limit>count and count < names[nowPage].size())
	{
		h += inter;
		Rect rect{ window.x + 10, h, a_w, a_h };
		if (rect.mouseOver())rect.draw(Palette::Gray);
		else rect.draw(Palette::White);
		font(names[nowPage][count].second).draw(rect, Palette::Black);
		count++;
		h += a_h;
	}

	Triangle{ Vec2(window.pos.x + window.w / 2 + 20,window.h + window.pos.y - inter - b_h),
		Vec2(window.pos.x + window.w / 2 + 20,window.h + window.pos.y - inter) ,
	Vec2(window.pos.x + window.w / 2 + 20 + b_w,window.h + window.pos.y - inter - b_h / 2) }.draw(Palette::Blue);

	Triangle{ Vec2(window.pos.x + window.w / 2 - 20,window.h + window.pos.y - inter - b_h),
		Vec2(window.pos.x + window.w / 2 - 20,window.h + window.pos.y - inter) ,
	Vec2(window.pos.x + window.w / 2 - 20 - b_w,window.h + window.pos.y - inter - b_h / 2) }.draw(Palette::Blue);
}

void Buttons::conect_button(String scene,String button, String next_scene)
{
	bool button_exist = false; bool scene_exist = false;
	for (auto butt : button_names[scene])if (butt == button)button_exist = true;
	if (button_names.contains(next_scene))scene_exist = true;

	if (button_exist and scene_exist)
	{
		comb_button.emplace(std::pair<String, String>(scene, button), next_scene);
	}
}

void Buttons::SetFirstScene(String scene)
{
	first_scene = scene;
	nowScene = first_scene;
}

void Buttons::SetButton(String scene, String name)
{
	if (button_names.contains(scene))
		button_names[scene] << name;
	else
		button_names.emplace(scene, Array<String>{name});
}

void Buttons::Reset() {
	selected.clear(); complete = false; nowScene = first_scene;
	active = false;
}

void Buttons::update()
{
	if (active && not complete) {
		int32 count = 0;
		bool pushed = false;
		for (auto button : button_names[nowScene])
		{
			if (SimpleGUI::Button(button, pos + Vec2{ 0,count * 45 }))
			{
				pushed = true;
				selected << button;
				if (comb_button.contains(std::pair<String,String>(nowScene,button)))
					nowScene = comb_button[std::pair<String, String>(nowScene, button)];
				else
					complete = true;
			}
			count++;
		}
		if (MouseL.down() && not pushed) { complete = true; }
	}
}

Display::Display()
{
	nowaction = Prenowaction = 0;
	actionId = 0;
	buttons.SetButton(U"条件", U"開始条件");
	buttons.SetButton(U"条件", U"続行条件");
	buttons.SetButton(U"条件", U"情報");
	buttons.SetButton(U"開始条件指定", U"Action開始からt秒たったら");
	buttons.SetButton(U"開始条件指定", U"あるActが開始してt秒たったら");
	buttons.SetButton(U"開始条件指定", U"あるActが終了してt秒たったら");
	buttons.SetButton(U"続行条件指定", U"t秒間");
	buttons.SetButton(U"続行条件指定", U"あるActが開始してからt秒たつまで");
	buttons.SetButton(U"続行条件指定", U"あるActが終了してからt秒たつまで");
	buttons.conect_button(U"条件", U"開始条件", U"開始条件指定");
	buttons.conect_button(U"条件", U"続行条件", U"続行条件指定");
	buttons.SetFirstScene(U"条件");
	mode = nomal;
	Cursor_r = 15;
	int32 AreaWidth, AreaHeight,
		action_window_w, action_window_h, act_window_w, act_window_h;
	Point action_window_pos, act_window_pos;

	AreaWidth = 800; AreaHeight = 600;
	action_window_w = Scene::Width() / 2, action_window_h = 100, action_window_pos = { 0,Scene::Height() - action_window_h };
	act_window_w = Scene::Width() / 2, act_window_h = 100, act_window_pos = { Scene::Center().x,Scene::Height() - act_window_h };

	EditArea = Rect{ Scene::Width() - AreaWidth,0,AreaWidth,AreaHeight };
	action_window = new ExplaWindow{ action_window_w,action_window_h,action_window_pos,Palette::White };
	act_window = new ExplaWindow{ act_window_w,act_window_h,act_window_pos,Palette::White };

	SaveArea = Rect{ 5,5,50,30 };
	OpenFileArea = Rect{ 65,5,50,30 };
	AddAction_Button = Rect{ 125,5,30,30 };
	seleWin = new ActionSelect(Scene::Width() - AreaWidth, AreaHeight - SaveArea.h - SaveArea.y * 2 - 5, Point{ 0,SaveArea.y * 2 + SaveArea.h });
	AddAction();

	action_window->Set_title(U"ActionName");
	action_window->Set_default_name(actions[0].Get_name(),&actions[0].name);
	act_window->Set_title(U"ActName");
};

void Display::AddAction()
{
	Action action;
	action.SetName(U"Action" + Format(actionId));
	seleWin->AddName(actionId, action.Get_name());
	nowaction = actionId;
	actions.emplace(actionId, action);
	actionId++;

	for (auto& action : actions)
	{
		act_window->Reset();
	}
}

void Display::Reset()
{
	actions.clear();
	nowaction = Prenowaction = 0;
	actionId = 0;
	delete action_window;
	delete act_window;
	delete seleWin;
	int32 AreaWidth, AreaHeight,
		action_window_w, action_window_h, act_window_w, act_window_h;
	Point action_window_pos, act_window_pos;

	AreaWidth = 800; AreaHeight = 600;
	action_window_w = Scene::Width() / 2, action_window_h = 100, action_window_pos = { 0,Scene::Height() - action_window_h };
	act_window_w = Scene::Width() / 2, act_window_h = 100, act_window_pos = { Scene::Center().x,Scene::Height() - act_window_h };

	EditArea = Rect{ Scene::Width() - AreaWidth,0,AreaWidth,AreaHeight };
	action_window = new ExplaWindow{ action_window_w,action_window_h,action_window_pos,Palette::White };
	act_window = new ExplaWindow{ act_window_w,act_window_h,act_window_pos,Palette::White };

	SaveArea = Rect{ 5,5,50,30 };
	OpenFileArea = Rect{ 65,5,50,30 };
	AddAction_Button = Rect{ 125,5,30,30 };
	seleWin = new ActionSelect(Scene::Width() - AreaWidth, AreaHeight - SaveArea.h - SaveArea.y * 2 - 5, Point{ 0,SaveArea.y * 2 + SaveArea.h });
	mode = nomal;
	
}
//jsonファイルにするとき、勝手にアルファベット順に並んでしまうため、開始/続行条件がActの中でぐちゃぐちゃになってしまう
//それを防ぐため、開始の条件はStartに、続行の条件はContinueに入れるようにした。
void Display::Save()
{
	JSON json;
	Array<JSON> Actions;
	Array<JSON> Actions_pos;
	for (auto& action : actions)
	{
		JSON Action;
		Array<JSON>Acts;
		JSON Action_pos;
		Array<JSON>Acts_pos;
		for (auto& act : action.second.acts)
		{
			JSON act_pos;
			act_pos[U"name"] = *act.second.act.Get_name_pointer();
			act_pos[U"startpos"] = act.second.GetStartPos();
			act_pos[U"endpos"] = act.second.GetLine().end;
			act_pos[U"number"] = act.first;
			Acts_pos << act_pos;

			JSON Act,Continue,Start;
			Array<JSON> Conection;
			Act[U"name"] = *act.second.act.Get_name_pointer();
			if (act.second.act.length_isUsed())	Continue[U"length"] = act.second.act.Get_length();
			else Continue[U"length"] = -1;
			for (auto& conec_end : action.second.conections_end[act.first])
			{
				JSON conection;
				conection[U"name"] = *action.second.acts[conec_end.arrow_id].act.Get_name_pointer();
				if (conec_end.point)conection[U"from"] = U"start";
				else conection[U"from"] = U"end";
				conection[U"time"] = conec_end.t;
				Conection << conection;
			}
			Continue[U"Conection"] = Conection;
			Conection.clear();
			if (act.second.act.start_time_isUsed()) Start[U"start_time"] = act.second.act.Get_start_time();
			else Start[U"start_time"] = -1;
			for (auto& conec_start : action.second.conections_start[act.first])
			{
				JSON conection;
				conection[U"name"] = *action.second.acts[conec_start.arrow_id].act.Get_name_pointer();;
				if (conec_start.point)conection[U"from"] = U"start";
				else conection[U"from"] = U"end";
				conection[U"time"] = conec_start.t;
				Conection << conection;
			}
			Start[U"Conection"] = Conection;
			Act[U"Continue"] = Continue;
			Act[U"Start"] = Start;
			Acts << Act;
		}
		Action[U"name"] = action.second.name;
		Action[U"Acts"] = Acts;
		Actions << Action;
		Action_pos[U"name"] = action.second.name;
		Action_pos[U"Acts_pos"] = Acts_pos;
		Actions_pos << Action_pos;
	}
	json[U"Actions"] = Actions;
	json[U"Pos"] = Actions_pos;

	Optional<FilePath> path = Dialog::SaveFile({ FileFilter::JSON() });
	if(path!=none)json.save(path.value());
}

void Display::Open()
{
	//アクションの形成
	Reset();

	Optional<FilePath> path = Dialog::OpenFile({ FileFilter::JSON() });
	
	if (path == none) {
		AddAction();
		action_window->Set_title(U"ActionName");
		action_window->Set_default_name(actions[0].Get_name(), &actions[0].name);
		act_window->Set_title(U"ActName");
		return;
	}

	const JSON json = JSON::Load(path.value());

	if (not json) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load" };
	}

	int32 n=0;
	for (const auto& Jaction : json[U"Actions"].arrayView())
	{
		AddAction();
		actions[n].SetName(Jaction[U"name"].getString());
		for (const auto& Jact : Jaction[U"Acts"].arrayView())
		{
			int32 num; String name; Point start, end;
			for (const auto& jpos : json[U"Pos"].arrayView())
			{
				if (jpos[U"name"] == Jaction[U"name"])
				{
					for (const auto jact_pos : jpos[U"Acts_pos"].arrayView())
					{
						if (jact_pos[U"name"] == Jact[U"name"])
						{
							num = jact_pos[U"number"].get<int32>();
							name = jact_pos[U"name"].getString();
							start = jact_pos[U"startpos"].get<Point>();
							end = jact_pos[U"endpos"].get<Point>();
						}
					}
				}
			}
			Act act;
			act.SetName(name);
			if (Jact[U"Start"][U"start_time"].get<double>() != -1)act.SetStart(Jact[U"Start"][U"start_time"].get<double>());
			if (Jact[U"Continue"][U"length"].get<double>() != -1)act.SetEnd(Jact[U"Continue"][U"length"].get<double>());
			for (const auto& conection : Jact[U"Start"][U"Conection"].arrayView())
			{
				for (const auto& jpos : json[U"Pos"].arrayView())
				{
					for (const auto jact_pos : jpos[U"Acts_pos"].arrayView())
					{
						if (jact_pos[U"name"] == conection[U"name"])
						{
							act.SetStart(act::Switch{ jact_pos[U"number"].get<int32>(),conection[U"name"].getString(),conection[U"time"].get<double>(),(conection[U"from"].getString() == U"start") });
						}
					}
				}
			}
			for (const auto& conection : Jact[U"Continue"][U"Conection"].arrayView())
			{
				for (const auto& jpos : json[U"Pos"].arrayView())
				{
					for (const auto jact_pos : jpos[U"Acts_pos"].arrayView())
					{
						if (jact_pos[U"name"] == conection[U"name"])
						{
							act.SetEnd(act::Switch{ jact_pos[U"number"].get<int32>(),conection[U"name"].getString(),conection[U"time"].get<double>(),(conection[U"from"].getString() == U"start") });
						}
					}
				}
			}
			Arrow arrow; 
			arrow.SetArrow(start, end, num, Palette::Black, Palette::White);
			arrow.act = act;
			actions[n].conections_start.emplace(num, arrow.act.Get_start());
			actions[n].conections_end.emplace(num, arrow.act.Get_end());
			actions[n].acts.emplace(num, arrow);
			actions[n].countArrow = Max(actions[n].countArrow, num + 1);
		}
		n++;
	}
	action_window->Set_title(U"ActionName");
	action_window->Set_default_name(actions[0].Get_name(), &actions[0].name);
	act_window->Set_title(U"ActName");
	nowaction = Prenowaction = 0;
}

void Display::change_nowaction()
{
	action_window->Reset();
	action_window->Set_name(&actions[nowaction].name);
	act_window->Reset();
}

void Display::update()
{
	Prenowaction = nowaction;

	seleWin->update();
	if (seleWin->Erase() != -1)
	{
		int32 count = 0;
		int32 num = 0;
		while (count < actions.size())
		{
			if (actions.contains(num))
			{
				if (num == seleWin->Erase())break;
				count++;
			}
			num++;
		}
		actions.erase(seleWin->Erase());
		if (actions.empty())
		{
			AddAction();
		}
		else if (not actions.contains(nowaction))
		{
			//もしnowactionのアクションが消されたら。
			if (count < actions.size())
			{
				while (not actions.contains(nowaction))nowaction++;
			}
			else
			{
				while (not actions.contains(nowaction))nowaction--;
			}
		}
	}
	if (seleWin->Select() != -1)
	{
		nowaction = seleWin->Select();
	}
	seleWin->NamesUpdate(actions);
	Action* action;
	action = &actions[nowaction];

	buttons.update();
	action->conectionUpdate();

	//	削除、マウスオーバーなど
	for (auto ite = action->acts.begin(); ite != action->acts.end();)
	{
		bool erased = false;
		switch(ite->second.mode)
		{
		case Arrow::nomal:
			if (Circle{ Cursor::Pos(),Cursor_r }.intersects(ite->second.GetLine()))ite->second.Mouse_over();
			break;
		case Arrow::mouse_over:
			if (not Circle{ Cursor::Pos(),Cursor_r }.intersects(ite->second.GetLine()))
			{
				ite->second.Nomal();
			}
			else if (MouseL.down())
			{
				if (mode != printbuttons and mode != timeInput)
				{
					//矢印が押されたときの処理
					//ただしボタン表示中は他矢印の選択を受け付けない
					if (mode == chooseAct)
					{
						//自分自身を選んでなければ、選択
						if (ite->second.mode !=Arrow::select)
						{
							ite->second.mode = Arrow::chosen;
							String exp;
							String button_str = buttons.Get_Selecting()[1];
							for (auto k : step(5))button_str.pop_front();
							exp = *ite->second.act.Get_name_pointer();
							exp += button_str;
							inp_window = new InputTimeWindow();
							inp_window->SetExplanation(exp);
							mode = timeInput;
						}
					}
					else
					{
						mode = selecting;
						ite->second.Select();
					}
				}
			}
			else if (MouseR.down())
			{
				act_window->Reset();
				ite = action->acts.erase(ite);
				action->conectionUpdate();
				erased = true;
			}
			else
			{
				act_window->Set_name(ite->second.act.Get_name_pointer());
			}
			break;
		}
		if(not erased)ite++;
	}

	//左クリックの処理
	if (MouseL.down())
	{
		if (SaveArea.mouseOver())Save();
		if (OpenFileArea.mouseOver())Open();
		if (AddAction_Button.mouseOver())AddAction();
	}
	if (MouseL.down() && Circle { Cursor::Pos(), 1 }.intersects(EditArea))
	{
		Arrow arrow;
		switch (mode)
		{
		case nomal:
			StartPos = Cursor::Pos();
			mode = set;
			break;
		case set:
			if (StartPos.x <= Cursor::Pos().x)
				arrow.SetArrow(StartPos, Point(Cursor::Pos().x, StartPos.y), action->countArrow, Palette::Black,Palette::White);
			else
				arrow.SetArrow(StartPos, StartPos, action->countArrow, Palette::Black, Palette::White);
			action->acts.emplace(action->countArrow, arrow);
			action->countArrow++;
			mode = nomal;
			act_window->Reset();
			break;
		case selecting:
			mode = printbuttons;
			//選択肢の表示
			buttons.Active((Point)(Cursor::Pos() + Point{ 10,-45 }));
			break;
		case printbuttons:
			if (buttons.is_complete()) mode = buttonsSelection;
			break;
		case chooseAct:
			//他のActをクリックした場合、上のfor文でmodeをselectingにされる
			mode = nomal;
			for (auto& arrow : action->acts)if (arrow.second.mode == Arrow::select)arrow.second.mode = Arrow::nomal;
			buttons.Reset();
			break;
		case showActinfo:
			mode = nomal;
			for (auto& arrow : action->acts)if (arrow.second.mode == Arrow::select)arrow.second.mode = Arrow::nomal;
			buttons.Reset();
			delete act_info;
			break;
		}
	}
	//ボタンの選択が終わったら
	if (mode == buttonsSelection)
	{
		if (buttons.Get_Selecting().size() >= 2)
		{
			if (buttons.Get_Selecting()[1] == U"Action開始からt秒たったら" or buttons.Get_Selecting()[1] == U"t秒間")
			{
				mode = timeInput;
				inp_window = new InputTimeWindow();
				inp_window->SetExplanation(buttons.Get_Selecting()[1]);
				for(auto arr:action->acts)
					if (arr.second.mode == Arrow::select and arr.second.act.time_using())
					{
						//すでに時間がセットされていた場合
						if (buttons.Get_Selecting()[1] == U"Action開始からt秒たったら")
							inp_window->SetTime(arr.second.act.Get_start_time());
						else
							inp_window->SetTime(arr.second.act.Get_length());
					}
			}
			else
			{
				mode = chooseAct;
			}
		}
		else if (buttons.Get_Selecting().size()==1)
		{
			if (buttons.Get_Selecting()[0] == U"情報")
			{
				mode = showActinfo;
				act_info = new InfoWindow(300, ColorF(Palette::Black, 0.2), &actions[nowaction]);
				act_info->SetPos(Point{ Scene::Width() - 300,0 });
			}
			else
			{
				mode = nomal;
				for (auto& arr : action->acts)if (arr.second.mode == Arrow::select)arr.second.mode = Arrow::nomal;
				buttons.Reset();
			}
		}
		else
		{
			mode = nomal;
			for (auto& arr : action->acts)if (arr.second.mode == Arrow::select)arr.second.mode = Arrow::nomal;
			buttons.Reset();
		}
	}
	if (nowaction != Prenowaction)change_nowaction();
}

void Display::draw()
{
	Action* action;
	action = &actions[nowaction];

	for (auto& arrow : action->acts)
	{
		arrow.second.draw();
	}

	for (auto& conection : action->conections_end)
	{
		for (auto& end_arrow : conection.second)
		{
			if (end_arrow.point)Line{ action->acts[conection.first].GetLine().end,action->acts[end_arrow.arrow_id].GetStartPos() }.draw(Palette::Green);
			else Line{ action->acts[conection.first].GetLine().end,action->acts[end_arrow.arrow_id].GetLine().end }.draw(Palette::Green);
		}
	}
	for (auto& conection : action->conections_start)
	{
		for (auto& start_arrow : conection.second)
		{
			if (start_arrow.point)Line{ action->acts[conection.first].GetStartPos(),action->acts[start_arrow.arrow_id].GetStartPos() }.draw(Palette::Green);
			else Line{ action->acts[conection.first].GetStartPos(),action->acts[start_arrow.arrow_id].GetLine().end }.draw(Palette::Green);
		}
	}

	if (mode == set)
	{	
		if (StartPos.x <= Cursor::Pos().x) Line{ StartPos,Point(Cursor::Pos().x, StartPos.y) }.drawArrow(5, Vec2{ 10,10 }, ColorF(Palette::Black, 0.7));
		Circle{ StartPos,6 }.draw(ColorF(Palette::Black, 0.7));
	}
//	EditArea.drawFrame(8, Palette::White);
	action_window->draw();
	act_window->draw();

	if (mode == showActinfo)
	{
		act_info->draw();
	}
	//時間入力
	if (mode == timeInput)
	{
		if (inp_window->draw())
		{
			act::Switch swi;
			for (auto& act : action->acts)if (act.second.mode == Arrow::chosen)
			{
				swi.actname = *act.second.act.Get_name_pointer();
				swi.arrow_id = act.first;
			}
			for (auto& arr : action->acts)
			{
				if (arr.second.mode == Arrow::select)
				{
					String button = buttons.Get_Selecting()[1];
					if (buttons.Get_Selecting()[0] == U"開始条件")
					{
						if (button == U"Action開始からt秒たったら")
						{
							arr.second.act.SetStart(inp_window->GetTime());
						}
						else
						{							
							if (button == U"あるActが開始してt秒たったら")
							{
								swi.point = true;
							}
							else if (button == U"あるActが終了してt秒たったら")
							{
								swi.point = false;
							}
							swi.t = inp_window->GetTime();
							if (not action->conections_start.contains(arr.first))
							{
								action->conections_start.emplace(arr.first, Array<act::Switch>{});
							}
							action->conections_start[arr.first] << swi;//*/
						}
					}
					else if (buttons.Get_Selecting()[0] == U"続行条件")
					{
						if (button == U"t秒間")
						{
							arr.second.act.SetEnd(inp_window->GetTime());
						}
						else
						{
							if (button == U"あるActが開始してからt秒たつまで")
							{
								swi.point = true;
							}
							else if (button == U"あるActが終了してからt秒たつまで")
							{
								swi.point = false;
							}
							swi.t = inp_window->GetTime();
							if (not action->conections_end.contains(arr.first))
							{
								action->conections_end.emplace(arr.first, Array<act::Switch>{});
							}
							action->conections_end[arr.first] << swi;//*/
						}
					}
				}
				arr.second.mode = Arrow::nomal;
			}
			delete inp_window;
			mode = nomal;
			buttons.Reset();
		}
	}

	SaveArea.draw(Palette::White);
	HOZON(U"保存").draw(SaveArea,Palette::Black);
	OpenFileArea.draw(Palette::White);
	HIRAKU(U"開く").draw(OpenFileArea, Palette::Black);
	AddAction_Button.draw(Palette::White);
	Shape2D::Plus(AddAction_Button.w/2 - 3, 2, AddAction_Button.center()).draw(Palette::Black);

	seleWin->draw();
}
