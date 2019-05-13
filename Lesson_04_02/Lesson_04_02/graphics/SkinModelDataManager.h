#pragma once
#include <map>

/*!
*@brief	スキンモデルデータマネージャー。。
*/
class SkinModelDataManager
{
public:
	/*!
	*@brief	モデルをロード。
	*@param[in]	filePath	ファイルパス。
	*/
	DirectX::Model* Load(const wchar_t* filePath);
	/*!
	*@brief	モデルデータを全開放。
	*/
	void Release();
private:
	//ファイルパスをキー、DirectXModelのインスタンスを値とするマップ。
	//辞書みたいなものです。
	std::map< 
		std::wstring,					//wstringがキー。 
		std::unique_ptr<DirectX::Model>	//これが値。
	> m_directXModelMap;
};

//g_skinModelDataManagerのextern宣言。
//extern宣言は実態ではないので注意！
//コンパイラにどこかにあるから使ってねと教えるもの。
extern SkinModelDataManager g_skinModelDataManager;

