#include "../../stdafx.h"
#include "TextureDraw.h"
#include"../../Characters/Character.h"

TextureDraw::TextureDraw(Texture texture, Character* chara, double scale)
	:texture(texture),chara(chara),scale(scale)
{
	this_Act_is_TextureDraw = true;
}

void TextureDraw::draw()
{
	if (chara->direction!=Vec2{-1,0})texture.scaled(scale).drawAt(chara->pos);
	else texture.scaled(scale).mirrored().drawAt(chara->pos);
}
