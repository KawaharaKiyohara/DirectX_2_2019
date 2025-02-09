// Lesson_03_02.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
//Hands-On 1 mapのヘッダーファイルをインクルード。
#include <map>

//Hands-On 2 stdをusingする。
using namespace std;

class ModelData {
};
int main()
{
	//Hands-On 3 文字列をキー、年齢を値に持つマップを作成。
	map<string, int> ageMap;
	//Hands-On 4 キーと値を登録する。
	ageMap.insert({ "大谷", 23 });
	ageMap.insert({ "田中", 29 });
	ageMap.insert({ "ダルビッシュ", 31});
	
	//Hands-On 5 キーを使って値を取得する。
	cout << ageMap.at("大谷") << "\n";
	cout << ageMap.at("田中") << "\n";
	cout << ageMap.at("ダルビッシュ") << "\n";

	//Hands-On 6 キーが存在するか調べる。
	auto it = ageMap.find("イチロー");
	//find関数が終端のイテレータを返して来たら、キーが見つからなかったということになる。
	if (it == ageMap.end()) {
		cout << "キーが見つからなかった。今回はここに来るのが正しい。\n";
	}
	it = ageMap.find("大谷");
	//キーが見つかれば、その要素が存在する場所を指すイテレータが返ってくる。
	if (it != ageMap.end()) {
		std::cout << "キーが見つかった。大谷の年齢は" << it->second << "です。\n";
	}
    return 0;
}

