#include "../stdafx.h"
#include "Acts.h"

void Acts::Start()
{
	if (not Working)
	{
		Working = true;
		timer->timeRestart();
		actSets.Clear();
		end = false;
	}
}

void Acts::update()
{
	if (Working)
	{
		for (auto& act : acts)
		{
			//startswitch
			if (act.second.start_time_use)
			{
				if (timer->time() >= act.second.start_time and (not start_switch[act.first].started))
				{
					start_switch[act.first].Start();
				}
			}
			//start
			if ((not act.second.mainAct->started) and (not act.second.mainAct->end) )
			{
				if (start_switch[act.first].started)
				{
					act.second.mainAct->Start();
					actSets.add_start(act.first, timer->time());
				}
				else
				{
					for (auto& trig : act.second.Trigger)
					{
						if (trig.trigger_type)
						{
							if (trig.from)
							{
								if (acts[trig.act_name].mainAct->started)
								{
									if (timer->time() >= actSets.StartTimeOf(trig.act_name) + trig.time)
									{
										act.second.mainAct->Start();
										actSets.add_start(act.first, timer->time());
									}
								}
							}
							else
							{
								if (acts[trig.act_name].mainAct->end)
								{
									if (timer->time() >= actSets.EndTimeOf(trig.act_name) + trig.time)
									{
										act.second.mainAct->Start();
										actSets.add_start(act.first, timer->time());
									}
								}
							}
						}
					}
				}
			}
			//switch update
			if (start_switch[act.first].started and (not start_switch[act.first].end))
			{
				start_switch[act.first].update();
			}
			if (end_switch[act.first].started and (not end_switch[act.first].end))
			{
				end_switch[act.first].update();
			}
			//update
			if (act.second.mainAct->started and (not act.second.mainAct->end))
			{
				act.second.mainAct->update();
				act.second.mainAct->update1();
			}
			if (act.second.mainAct->end1)
			{
				if ((not end_switch[act.first].started))end_switch[act.first].Start();
			}
			//endswitch
			if (act.second.length_use)
			{
				if (act.second.mainAct->get_time() >= act.second.length and (not end_switch[act.first].started))
				{
					end_switch[act.first].Start();
				}
			}
			//end
			if (act.second.mainAct->started and (not act.second.mainAct->end))
			{
				if (end_switch[act.first].started)
				{
					act.second.mainAct->endAct();
					act.second.mainAct->endAct1();
					actSets.add_end(act.first, timer->time());
				}
				else
				{
					for (auto& trig : act.second.Trigger)
					{
						if (not trig.trigger_type)
						{
							if (trig.from)
							{
								if (acts[trig.act_name].mainAct->started)
								{
									if (timer->time() >= actSets.StartTimeOf(trig.act_name) + trig.time)
									{
										act.second.mainAct->endAct();
										act.second.mainAct->endAct1();
										actSets.add_end(act.first, timer->time());
									}
								}
							}
							else
							{
								if (acts[trig.act_name].mainAct->end)
								{
									if (timer->time() >= actSets.EndTimeOf(trig.act_name) + trig.time)
									{
										act.second.mainAct->endAct();
										act.second.mainAct->endAct1();
										actSets.add_end(act.first, timer->time());
									}
								}
							}
						}
					}
				}
			}
		}
		timer->update();
		if (actSets.EndActNum() >= numAct)end=true;
	}
}

void Acts::draw()
{
	if (Working)
	{
		for (const auto& act : acts)
		{
			if (act.second.mainAct->started and (not act.second.mainAct->end))
			{
				if(not act.second.mainAct->this_Act_is_TextureDraw)act.second.mainAct->draw();
			}
		}
	}
}

void Acts::drawTexture()
{
	if (Working)
	{
		for (const auto& act : acts)
		{
			if (act.second.mainAct->started and (not act.second.mainAct->end))
			{
				if(act.second.mainAct->this_Act_is_TextureDraw) act.second.mainAct->draw();
			}
		}
	}
}

void Acts::End()
{
	for (auto& act : acts)act.second.mainAct->Reset();
	for (auto& swi : start_switch)swi.second.Reset();
	for (auto& swi : end_switch)swi.second.Reset();
	Working = false;
	actSets.Clear();
	end = false;
}

void Acts::StartAct(String Name)
{
	if (Working)
	{
		if (start_switch.contains(Name))
		{
			if (not start_switch[Name].started)
			{
				if(acts[Name].mainAct->end)start_switch[Name].Start();
			}
		}
	}
}

void Acts::EndAct(String Name)
{
	if (Working)
	{
		if (end_switch.contains(Name))
		{
			if (not end_switch[Name].started)
			{
				if(acts[Name].mainAct->started)
					end_switch[Name].Start();
			}
		}
	}
}

