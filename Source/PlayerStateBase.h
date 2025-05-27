#pragma once
#include "Player.h"
class Player;

class PlayerState
{
public:
    // �R���X�g���N�^
    PlayerState(Player* player) : owner(player) {}
    // �f�X�g���N�^
    virtual ~PlayerState() {}
    //�S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    //�X�e�[�g�ɓ��������̃��\�b�h
    virtual void Enter() = 0;
    //�X�e�[�g�Ŏ��s���郁�\�b�h
    virtual void Execute(float elapsedTime) = 0;
    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    virtual void Exit() = 0;
protected:
    Player* owner;
};