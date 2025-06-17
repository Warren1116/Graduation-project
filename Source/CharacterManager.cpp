#include "CharacterManager.h"
#include "Collision.h"

// 更新処理
void CharacterManager::Update(float elapsedTime)
{
    for (Character* character : characters)
    {
        if (character != nullptr && !character->GetAlive())
        {
            removes.emplace_back(character);

        }
    }

    // 破棄処理
    // ※enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
    // 　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
    for (Character* character : removes)
    {
        // std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Character*>::iterator it = std::find(characters.begin(), characters.end(), character);
        if (it != characters.end())
        {
            characters.erase(it);
        }

    }

    // 破棄リストをクリア
    removes.clear();

    // キャラクター同士の衝突処理
    CollisionCharacterVsCharacter();
}

// デバッグプリミティブ描画
void CharacterManager::DrawDebugPrimitive()
{
    for (Character* character : characters)
    {
        character->DrawDebugPrimitive();
    }
}

// デバッグ用GUI描画
void CharacterManager::DrawDebugGUI()
{
    for (Character* character : characters)
    {
        character->DrawDebugGUI();
    }
}

// キャラクター登録
void CharacterManager::Register(Character* character)
{
    characters.emplace_back(character);
}

// キャラクター全削除
void CharacterManager::Clear()
{
    characters.clear();
    removes.clear();
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


}

// キャラクター同士の衝突処理
void CharacterManager::CollisionCharacterVsCharacter()
{
    size_t count = characters.size();
    for (int i = 0; i < count; ++i)
    {
        Character* characterA = characters.at(i);
        for (int j = i + 1; j < count; ++j)
        {
            Character* characterB = characters.at(j);

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
