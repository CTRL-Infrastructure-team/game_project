#pragma once

class Background
{
public:
	Texture texture;
	double r, rate;
	uint32 num;
	Background(String pass, int x, uint32 n = 1) :texture{ pass } {
		r = Scene::Size().y / (double)texture.height();//倍率
		rate = (texture.width() * r * n - Scene::Size().x) / (rect_size * x);//画像を進める速度
		num = n;
	}
	void draw(Vec2 vec) {
		for (auto n : step(num)) {
			texture.scaled(r).draw(-vec.x * rate + (texture.width() * r) * n, 0);
		}
	}

};


//何ブロックまで描画するか
constexpr int range_right = 15;
constexpr int range_left = 10;

class Stage
{
public:
	Grid<Object*>map;

	Stage(String path) {

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
		this->map = map;
	}

	void draw(Vec2 vec) {
		//ブロックの描写
		Point pos = vec.asPoint() / Point(rect_size, rect_size);//プレイヤーの座標をマップ番号に変換
		for (int y = 0; y < map.height(); y++)
		{
			for (int x = Max(0, pos.x - range_left); x < Min((int)map.width(), pos.x + range_right); x++) {
				Point p(x, y);
				if (map[p] != NULL)map[p]->draw(p);
			}
		}

	}
	Grid < Object*>* getMap() { return &map; }
	int width() { return (int)map.width(); }
};
