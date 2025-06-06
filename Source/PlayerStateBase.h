#pragma once
#include "Player.h"
class Player;

class PlayerState
{
public:
    // コンストラクタ
    PlayerState(Player* player) : owner(player) {}
    // デストラクタ
    virtual ~PlayerState() {}
    //全て継承先で実装させる必要があるため純粋仮想関数で実装
    //ステートに入った時のメソッド
    virtual void Enter() = 0;
    //ステートで実行するメソッド
    virtual void Execute(float elapsedTime) = 0;
    //ステートから出ていくときのメソッド
    virtual void Exit() = 0;
protected:
    Player* owner;
};