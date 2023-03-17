#pragma once
#include "Act.h"

class Character;

class TextureDraw :
	public Act
{
public:
	TextureDraw(Texture texture,Character* chara,double scale);
	~TextureDraw() {};

	void draw();
private:
	Texture texture;
	Character* chara;
	double scale = 1;
};
