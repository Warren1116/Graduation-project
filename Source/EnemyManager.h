#pragma once

#include <vector>
#include "Enemy.h"
#include <set>

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    // �B��̃C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    int identity = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)

    //�@�֐�
    void Update(float elapsedTime);	// �X�V����
    void Register(Enemy* enemy);	// �G�l�~�[�o�^
    void Clear();	// �G�l�~�[�S�폜
    void DrawDebugPrimitive();	// �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugGUI();    // �f�o�b�OGUI�`��
    void Remove(Enemy* enemy);    // �G�l�~�[�폜

    ///////////////////////////////////////////////////////////////////////////////////
    //                              �Q�b�^�[�E�Z�b�^�[                               //
    ///////////////////////////////////////////////////////////////////////////////////
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }   // �G�l�~�[���擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }    // �G�l�~�[�擾
    Model* GetEnemyModel(int index);    // �G�l�~�[���f���擾
    Enemy* GetEnemyFromId(int id);    // �t�^����ID����G�l�~�[���擾

private:
    std::vector<Enemy*> enemies;
    std::set<Enemy*> removes;

    // �G�l�~�[���m�̏Փˏ���
    void CollisionEnemyVsEnemies();
};

