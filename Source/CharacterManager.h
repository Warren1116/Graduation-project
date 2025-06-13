#pragma once

#include <vector>
#include "Graphics/Shader.h"
#include "Character.h"

// キャラクターマネージャー
class CharacterManager
{
private:
	CharacterManager() {}
	~CharacterManager() {}

public:
	// 唯一のインスタンス取得
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

	// キャラクター登録
	void Register(Character* enemy);

	// キャラクター全削除
	void Clear();

	// キャラクター削除
	void Remove(Character* enemy);

	// キャラクター数取得
	int GetCharacterCount() const { return static_cast<int>(characters.size()); }

	// キャラクター取得
	Character* GetCharacter(int index) { return characters.at(index); }

private:
	// キャラクター同士の衝突処理
	void CollisionCharacterVsCharacter();

private:
	std::vector<Character*>	characters;
	std::vector<Character*>	removes;
};
