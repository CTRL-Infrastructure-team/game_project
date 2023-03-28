#pragma once
#include"Action.h"
#include"c_timer.h"
#include"Act.h"

constexpr int32 fontsize_Expla_title = 15;
constexpr int32 fontsize_Info = 15;

class InputTimeWindow
{
public:
	InputTimeWindow();
	~InputTimeWindow();

	void SetExplanation(String text);
	void SetTime(double t);
	double GetTime();

	bool draw();

private:
	String expla;
	double time;
	TextEditState tex;
	Font font{ 15 };
	Point pos = Scene::Center();
	int32 h = 200;
	int32 w = 300;
	Rect close;
};

class ExplaWindow
{
public:
	ExplaWindow(int32 width, int32 height, Point pos, ColorF color) :
		color(color), width(width), height(height), pos(pos)
	{
		TextBoxSize = 300;
	}
	virtual ~ExplaWindow() {}

	virtual void draw();

	void Set_title(String Title) { title = Title; };

	void Set_name(String* Name) {
		if (p_name != Name) {
			p_name = Name;
			name.text = *Name;
		}
	}

	void Set_default_name(String def, String* Name)
	{
		p_name = Name;
		name.text = def;
	}
	void Set_infomation(String Info) { infomation << Info; };

	virtual void Reset(){
		p_name = NULL;
		name.text.clear();
	}

protected:
	Font f_title{ fontsize_Expla_title };
	ColorF color;
	Point pos;
	int32 width;
	int32 height;
	String title;
	TextEditState name;
	TextEditState memo;
	int32 TextBoxSize;
	Array<String> infomation;
	String* p_name;
};

class InfoWindow :public ExplaWindow
{
public:
	InfoWindow(int32 width, ColorF color, Action* action) :ExplaWindow(width, 0, Point{ 0,0 }, color), action(action) {}
	~InfoWindow() {}

	void draw();

	void SetPos(Point Pos) {
		pos = Pos;
	};

	void Reset() {
		infomation.clear();
		p_name = NULL;
		name.text.clear();
	}

	void AddInfomation(String info) { infomation << info; }

private:
	Font f_info{ fontsize_Info };
	Action *action;
};

class ActionSelect
{
public:
	ActionSelect(int32 width, int32 height, Point pos);
	~ActionSelect();

	void AddName(int32 id,String name)
	{
		if ( names[lastPage].size() >= Limit)lastPage++;
		names[lastPage] << std::pair<int32,String>{id, name};
		nowPage = lastPage;
	}
	void update();

	int32 Erase();
	int32 Select();
	void NamesUpdate(HashTable<int32, Action> Actions);

	void draw();
private:
	Rect window;
	int32 a_w, a_h, b_w, b_h;
	int32 inter;
	HashTable < int32, Array<std::pair<int32, String>>> names;//page,Array<id,name>
	int32 nowPage;
	int32 lastPage;
	int32 Limit;
	Font font{ 20 };

};

class Buttons
{
public:
	Buttons() { active = false; complete=false; }
	~Buttons() {}

	void Active(Point Pos) { active = true; pos = Pos; }
	bool is_complete() { return complete; }
	Array<String> Get_Selecting() { return selected; }
	void SetFirstScene(String scene);
	void conect_button(String scene,String button, String next_scene);
	void unActive() { active = false; }
	void update();
	void SetButton(String scene, String button_name);

	void Reset();

private:
	String first_scene;
	Point pos;
	bool active;
	bool complete;
	String nowScene;
	HashTable<std::pair<String,String>,String>comb_button;
	HashTable<String, Array<String>>button_names;
	Array<String> selected;
};

class Display
{
public:
	Display();
	~Display() {};

	void AddAction();

	void change_nowaction();

	void Save();

	void Reset();

	void Open();

	void update();
	void draw();
private:

	enum Mode {
		nomal,
		set,
		selecting,
		printbuttons,
		buttonsSelection,
		timeInput,
		chooseAct,
		showActinfo,
	}mode;
	Point StartPos;
	int32 nowaction;
	int32 Prenowaction;
	HashTable<int32, Action> actions;
	Rect EditArea;
	ExplaWindow* action_window;
	ExplaWindow* act_window;
	int32 Cursor_r;
	Buttons buttons;
	InfoWindow* act_info;
	InputTimeWindow* inp_window;
	Rect SaveArea;
	Rect OpenFileArea;
	Font HIRAKU{ 20 };
	Font HOZON{ 20 };
	Rect AddAction_Button;
	ActionSelect *seleWin;
	int32 actionId;
};

