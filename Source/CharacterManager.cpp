#include "CharacterManager.h"
#include "Collision.h"

// 更新処理
void CharacterManager::Update(float elapsedTime)
{
	for (Character* character : characteres)
	{
		if (character != nullptr)
		{
			if (!character->GetAlive())
			{
				removes.emplace_back(character);
			}

		}
	}

	// 破棄処理
	// ※enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	// 　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (Character* character : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Character*>::iterator it = std::find(characteres.begin(), characteres.end(), character);
		if (it != characteres.end())
		{
			characteres.erase(it);
		}

		//// 削除
		//delete character;
	}
	// 破棄リストをクリア
	removes.clear();

	// キャラクター同士の衝突処理
	CollisionCharacterVsCharacter();
}

// デバッグプリミティブ描画
void CharacterManager::DrawDebugPrimitive()
{
	for (Character* character : characteres)
	{
		character->DrawDebugPrimitive();
	}
}

// デバッグ用GUI描画
void CharacterManager::DrawDebugGUI()
{
	for (Character* character : characteres)
	{
		character->DrawDebugGUI();
	}
}

// キャラクター登録
void CharacterManager::Register(Character* character)
{
	characteres.emplace_back(character);
}

// キャラクター全削除
void CharacterManager::Clear()
{
	for (Character* character : characteres)
	{
		delete character;
	}
	characteres.clear();
}

// キャラクター削除
void CharacterManager::Remove(Character* character)
{
	// 破棄リストにすでにあれば弾く
	if (std::find(removes.begin(), removes.end(), character) == removes.end())
	{
		// 破棄リストに追加
		removes.emplace_back(character);
	}

	//for (auto& it : removes)
	//{
	//	if (it == character)
	//		break;
	//}
	//// 破棄リストに追加
	//removes.emplace_back(character);
}

// キャラクター同士の衝突処理
void CharacterManager::CollisionCharacterVsCharacter()
{
	size_t count = characteres.size();
	for (int i = 0; i < count; ++i)
	{
		Character* characterA = characteres.at(i);
		for (int j = i + 1; j < count; ++j)
		{
			Character* characterB = characteres.at(j);

			DirectX::XMFLOAT3 outPositionA, outPositionB;
			if (Collision::IntersectSphereVsSphere(
				characterA->GetPosition(),
				characterA->GetRadius(),
				characterB->GetPosition(),
				characterB->GetRadius(),
				outPositionA,
				outPositionB))
			{
				characterA->SetPosition(outPositionA);
				characterB->SetPosition(outPositionB);
			}
		}
	}
}
