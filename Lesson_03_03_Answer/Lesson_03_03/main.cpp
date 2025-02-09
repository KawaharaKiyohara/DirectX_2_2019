// Lesson_03_02.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <map>

using namespace std;

int main()
{
	//文字列をキー、年齢を値とするマップ。
	map<string, int> ageMap;
	//ゲームループ
	while (true) {
		char name[256];
		cout << "===================================\n";
		cout << "名前を入力してください。\n";
		cin >> name;
		auto it = ageMap.find(name);
		if (it == ageMap.end()) {
			cout << "新規ご登録ですね。年齢を入力してください。\n";
			int age;
			cin >> age;
			ageMap.insert({name, age });
		}
		else {
			cout << "あなたは登録済みです。あなたの年齢は" << it->second << "ですね。\n";
		}
	}
    return 0;
}

