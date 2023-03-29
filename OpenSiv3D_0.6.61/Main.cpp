#include <Siv3D.hpp>
#include"Object.h"
#include"HitBox.h"
#include"Characters/Character.h"
#include"Characters/Player/Player.h"
#include"Characters/Enemy/Enemy.h"
#include"Block.h"
#include"Stage.h"

void Main()
{
	//各種設定
	Window::Resize(1000, 600);
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	Array<Character*>list;
	Stage stage{ U"tutorial.json" };
	Background background1{ U"背景.png" ,stage.width() };
	Background background2{ U"木背景.png" ,stage.width(),3 };
	bool SystemUpdate = false;
	double Gravity = 1000;
	//プレイヤーの生成
	Player player(Vec2{ 500, 350 }, Vec2{ 0,0 }, Vec2{ 0,0 },RectF{ 0,0,60,110 }, &list, &SystemUpdate);
	
	for (auto& chara : list)
	{
		chara->Set_G(&Gravity);//重力加速度をセット
	}
	
	while (System::Update())
	{
		ClearPrint();
		if (KeyZ.down())
		{
			(new Snake(RectF{0,0,60,50}, &list, &SystemUpdate, Vec2{ 500,350 }, Vec2{ 100,0 }))
				->Set_G(&Gravity);//重力加速度セット
		}
		for (auto& chara : list)
		{
			chara->box.stage(stage.getMap());
			chara->update();
			chara->ActionsUpdate();
			chara->move();
			chara->box.update();
		}

		background1.draw(player.pos);
		background2.draw(player.pos);

		//落下したらスタートに戻す。(デバッグ用)
		if (player.pos.y > 1000)player.pos=Vec2(500, 350);

		{
			//座標変換
			Mat3x2 mat = Mat3x2::Translate(Vec2(500 - player.pos.x, 0));
			const Transformer2D transformer{ mat, TransformCursor::Yes };

			//描写
			for (const auto& chara : list)
			{
				chara->draw_nonvirtual();
			}
			stage.draw(player.pos);
		}
		SystemUpdate = not(SystemUpdate);
	}
}

