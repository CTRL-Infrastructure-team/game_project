#pragma once
constexpr double unUse = -1;

namespace act {
	struct Switch;
}

class Arrow;

class Act
{
public:
	Act();
	~Act();

	void SetName(String name);
	void SetStart(act::Switch start);
	void SetStart(double time) { start_time = time; time_used = true; }
	void ReSetStartTime() { start_time = unUse; if (length == unUse)time_used = false; }
	void ReSetLengthTime() { length = unUse; if (start_time == unUse)time_used = false; }
	void SetEnd(act::Switch end);
	void SetEnd(double time) { length = time; time_used = true; }

	bool time_using() { return time_used; }
	double Get_start_time() { if (start_time == unUse)return 0; else return start_time; }
	double Get_length() { if (length == unUse)return 0; else return length; }
	bool start_time_isUsed() { return start_time != unUse; }
	bool length_isUsed() { return length != unUse; }
	String* Get_name_pointer() { return &name; }
	Array<act::Switch> Get_start() { return start; }
	Array <act::Switch> Get_end() { return end; }
private:
	String name;
	Array<act::Switch> start;
	Array<act::Switch> end;
	bool time_used=false;
	double start_time = unUse;//アクション開始から何秒後か
	double length = unUse;//アクト開始から何秒間か
};

