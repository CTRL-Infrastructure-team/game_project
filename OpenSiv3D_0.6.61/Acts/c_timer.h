#pragma once

class c_timer
{
private:
	//時間（秒）
	double time_s;
	//周期（秒）
	double periodTime;
	//Preiod関数内で動く時間
	double periodTimer;
	//制限時間関数が動作中かどうか
	bool LimitTimerRunning = false;
	//制限時間
	double LimitTime;
	//制限時間関数で動く時間
	double LimitTimer;
	//制限時間関数
	void Limit() {if (LimitTimer >= LimitTime)LimitTimerRunning = false; }

	bool* SystemUpdate_id;
	//updateがsystemupdateのループ内で一度しか呼ばれないようにする工夫
	std::pair<bool, bool> already_updated_in_this_SystemUpdate;//updated or not,SystemUpdate_id
public:
	c_timer(bool* systemupdate, double limit_t = 1, double time = 0, double periodTimer = 0, double periodTime = 1)
		:time_s(time), periodTimer(periodTimer), periodTime(periodTime), LimitTimer(limit_t), LimitTime(limit_t) {
		SystemUpdate_id = systemupdate;
		already_updated_in_this_SystemUpdate = std::pair<bool, bool>(false, SystemUpdate_id);
	};
	//周期periodTimeでtrueになる
	bool Period();
	void StartLimitTimer() { if (not LimitTimerRunning) { LimitTimerRunning = true; LimitTimer = 0; } }
	void ReStartLimitTimer() { LimitTimerRunning = true;LimitTimer = 0; }
	void StartLimitTimer(double timelimit) { LimitTime = timelimit; if (not LimitTimerRunning) { LimitTimerRunning = true; LimitTimer = 0; } }
	double GetLimitTimerRunning() { return LimitTimerRunning; }
	void End() { LimitTimerRunning = false; };
	//現在の経過時間
	double GetLimitTimer() { return LimitTimer; };
	double GetLimitTime() { return LimitTime; }
	//制限時間の割合
	double GetLimitTimerPerLimitTime() { if (not LimitTime == 0)return LimitTimer / LimitTime; else return -1; };
	//time_sをリスタート
	void timeRestart(double t = 0) { time_s = t; };

	double time() { return time_s; }

	void update(bool stop = false);
};


