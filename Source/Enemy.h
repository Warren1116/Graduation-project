#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Telegram.h"

// �O���錾
class EnemyManager;

// �G�l�~�[
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    // �֐�
    virtual void Update(float elapsedTime) = 0;	    // �X�V����
    void Destroy();	                                // �j��
    virtual void DrawDebugPrimitive();	            // �f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugGUI() {}                  // �f�o�b�OGUI�`��
    virtual bool OnMessage(const Telegram& msg);	// ���b�Z�[�W��M�����Ƃ��̏���
    void OnDamaged() override {};	                // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDead() override {};	                    // ���S�����Ƃ��ɌĂ΂��

    ///////////////////////////////////////////////////////////////////////////////////
    //                              �Q�b�^�[�E�Z�b�^�[                               //
    ///////////////////////////////////////////////////////////////////////////////////
    virtual bool	GetAttackFlg() { return attackFlg; }	        // �U���t���O�擾
    virtual void	SetAttackFlg(bool flg) { attackFlg = flg; };	// �U���t���O�Z�b�g
    virtual void	SetId(int id) { this->id = id; }
    virtual int		GetId() { return id; }
    virtual float GetAttackRange() { return attackRange; }	        // �U���͈͎擾
    Model* GetModel() const { return model; }	                    // ���f���擾

protected:
    Model* model = nullptr;
    bool		camerashake = false;
    EnemyManager* manager = nullptr;

    int	state = 0;
    bool attackFlg = false;
    int id = 0;
    float searchRange = 0.0f;
    float attackRange = 0.0f;

};

