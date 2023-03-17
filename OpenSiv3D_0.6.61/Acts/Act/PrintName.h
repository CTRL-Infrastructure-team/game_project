#pragma once
#include"Act.h"

class PrintString:public Act
{
public:
	PrintString();
	~PrintString();

	void SetString(const String& Str) { str = Str; }

	void update();
private:
	String str;
};

