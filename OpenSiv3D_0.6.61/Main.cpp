#include <Siv3D.hpp>
#include"Object.h"
#include"HitBox.h"
#include"Characters/Character.h"
#include"Characters/Player/Player.h"
#include"Characters/Enemy/Enemy.h"
#include"Block.h"

Grid<Object*> MyLoad(String path);

void Main()
{
	//各種設定
	Window::Resize(1000, 600);
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	Array<Character*>list;
	Grid<Object*>map = MyLoad(U"test.json");
	bool SystemUpdate = false;
	double Gravity = 1000;
	//プレイヤーの生成
	Player player(Vec2{ 500, 350 }, Vec2{ 0,0 }, Vec2{ 0,0 },RectF{ Arg::center(0,0),60,110 }, &list, &SystemUpdate);
	
	for (auto& chara : list)
	{
		chara->Set_G(&Gravity);//重力加速度をセット
	}
	
	while (System::Update())
	{
		ClearPrint();
		if (KeyZ.down())
		{
			(new Snake(HitBox{ new Vec2{},new Vec2{},RectF{Arg::center(0,0),60,50} }, &list, &SystemUpdate, Vec2{ 500,350 }, Vec2{ 100,0 }))
				->Set_G(&Gravity);//重力加速度セット
		}
		for (auto& chara : list)
		{
			chara->box.stage(&map);
			chara->update();
			chara->ActionsUpdate();
			chara->move();
			chara->box.update();
		}

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

			//ブロックの描写
			int range = 10;//プレイヤーから何ブロックを描写するか
			Point pos = player.pos.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
			for (int y = 0; y < map.height(); y++)
			{
				for (int x = Max(0, pos.x - range); x < Min((int)map.width(), pos.x + range); x++) {
					Point p(x, y);
					if (map[p] != NULL)map[p]->draw(p);
				}
			}
		}
		SystemUpdate = not(SystemUpdate);
	}
}


Grid<Object*> MyLoad(String path) {

	//辞書を作成
	HashTable<String, Object*> table;
	for (const auto& object : objects)table.emplace(object->name, object);

	//ファイルをロード
	JSON json = JSON::Load(path);
	if (not json)throw Error{ U"ファイルを読み込めませんでした。" };

	//ファイルにあるブロック名を読み込む(ステージで使われていないブロックを読み込もうとしてエラーになるのを防ぐため)
	Array<String>material_name;
	for (const auto& object : json[U"object"]) {
		material_name << object.key;
	}

	//ステージサイズのmapを用意
	Grid<Object*>map(json[U"stage_size"].get<Point>(), NULL);

	//mapにファイルのデータを読み込む。
	for (int i = 0; i < material_name.size(); i++) {
		for (const auto& elem : json[U"object"][material_name[i]].arrayView()) {
			map[elem.get<Point>()] = table[material_name[i]];
		}
	}

	return map;
}
