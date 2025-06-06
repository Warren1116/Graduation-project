#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "EnemyStateMachine.h"
#include "Telegram.h"
#include "Audio/Audio.h"

// �G�l�~�[�A�j���[�V����
enum class EnemyAnimation {
    Idle, Walk, Run, AttackShot, Die, GetHit,
    AttackPunch, GetThrow, HoldGun, KickDown
};

//�X�e�[�g�}�V��
class EnemyStateMachine;

class EnemyThief :public Enemy
{
public:
    EnemyThief();
    ~EnemyThief()override;

    // �X�e�[�g
    enum class State { Search, Battle, Recieve, Dead, Max };  //�e�X�e�[�g
    enum class Search { Wander, Idle, Max };            //�q (Search�p)
    enum class Battle { Pursuit, Attack, Punch, Shot,
        Damage, Standby, Max };                   //�q (Battle�p)
    enum class Recieve { Called, Max };                 //�q�iMetaAI�p�j

    enum class AttackType { Null, Punch, Shot };        //�U���̎��
    AttackType randomType = AttackType::Null;

    float				stateTimer = 0.0f;  // �X�e�[�g�^�C�}�[

    //�֐�
    void Update(float elapsedTime)override;             // �X�V����
    void DrawDebugPrimitive() override;                 // �f�o�b�O�v���~�e�B�u�`��
    void SetTerritory(const DirectX::XMFLOAT3& origin,
        float range);                                   // �꒣��ݒ�
    void DrawDebugGUI();                                // �f�o�b�O�G�l�~�[���\��
    void SetRandomTargetPosition();                     // �^�[�Q�b�g�ʒu�������_���ݒ�
    void MoveToTarget(float elapsedTime,
        float speedRate);                               // �ڕW�n�_�ֈړ�
    bool SearchPlayer();                                // �v���C���[���G
    void SetTargetPosition(DirectX::XMFLOAT3 position)
    { targetPosition = position; }                      // �^�[�Q�b�g�|�W�V�����ݒ�
    void OnDead();                                      // ���S�����Ƃ��ɌĂ΂��      
    void OnDamaged() override;                          // �_���[�W���󂯂��Ƃ��ɌĂ΂��
    bool OnMessage(const Telegram& msg);                //�@���b�Z�[�W��M�֐���ǉ�

    ///////////////////////////////////////////////////////////////////////////////////
    //                              �Q�b�^�[�E�Z�b�^�[                               //
    ///////////////////////////////////////////////////////////////////////////////////
    DirectX::XMFLOAT3 GetPosition() { return position; }    // �|�W�V�����擾

    //�U��
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }        // �^�[�Q�b�g�|�W�V�����擾
    float GetAttackRange() { return attackRange; }    // �U���͈͎擾
    float GetPunchRange() { return punchRange; }    // �p���`�͈͎擾

    //���������
    void SetThrow(bool thown) { GetThrowFlag = thown; }
    bool IsGetThrow() const { return GetThrowFlag; }

    //�X�e�[�g�}�V��
    EnemyStateMachine* GetStateMachine() { return stateMachine; }    //�X�e�[�g�}�V���擾

    //���f��
    Model* GetModel() { return model.get(); }    //���f���擾

    //�X�e�[�g
    void SetStateTimer(float timer) {stateTimer = timer;}    // �X�e�[�g�^�C�}�[�ݒ�
    float GetStateTimer() { return stateTimer; }    // �X�e�[�g�^�C�}�[�擾
private:
    // ���f��
    std::unique_ptr<Model> model;

    // �X�e�[�g�}�V��
    EnemyStateMachine* stateMachine = nullptr;

    //�X�e�[�g
    State				state = State::Search;

    //���G
    DirectX::XMFLOAT3	targetPosition = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
    float				territoryRange = 7.0f;
    float				searchRange = 7.0f;

    //�ړ�
    float				moveSpeed = 3.5f;
    float				turnSpeed = DirectX::XMConvertToRadians(360);

    //�U��
    float				attackRange = 10.0f;
    float				punchRange = 1.5f;

    //��������鎞�p
    float timeElapsed = 0.0f;
    float totalFlightTime;
    float webTimer;
    bool GetThrowFlag;

    float deadTimer = 0.0f;    // ���S���̃^�C�}�[
    float deadTimerMax = 120.0f;    // ���S���̃^�C�}�[�ő�l


};