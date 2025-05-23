#pragma once
#include "Player.h"
class Player;

class PlayerState
{
public:
    PlayerState(Player* player) : owner(player) {}
    virtual ~PlayerState() {}
    virtual void Enter() = 0;
    virtual void Execute(float elapsedTime) = 0;
    virtual void Exit() = 0;
protected:
    Player* owner;
};