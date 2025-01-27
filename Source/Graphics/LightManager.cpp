#include "LightManager.h"
#include "imgui.h"

LightManager::LightManager()
{
	// ��e�N�X�`����ǂݍ���ł���
	// mipmap�̍쐬�ɂ��Ă��`������
	
	//sky
	skyTexture = std::make_unique<Texture>("Data/Texture/incskies_046_16k.jpg");
	//city
	//skyTexture = std::make_unique<Texture>("Data/Texture/buikslotermeerplein.jpg");

}

LightManager::~LightManager()
{
	Clear();
}

//�@���C�g��o�^
void LightManager::Register(Light* light)
{
	lights.emplace_back(light);
}

//�@���C�g���폜
void LightManager::Remove(Light* light)
{
	std::vector<Light*>::iterator	it = std::find(lights.begin(), lights.end(), light);
	if (it != lights.end())
	{
		lights.erase(it);
		delete	light;
	}
}

void LightManager::Clear()
{
	for (Light* light : lights)
	{
		delete	light;
	}
	lights.clear();
}

// �����_�����O�R���e�L�X�g�Ƀ��C�g����ǉ�
void LightManager::PushRenderContext(RenderContext& rc)
{
	// �����̏���ǉ�
	rc.ambientLightColor = ambientColor;

	//	�X�J�C�{�b�N�X�e�N�X�`����ǉ�
	if (skyTexture)
		rc.skyboxData.skybox = skyTexture->GetShaderResourceView().Get();

	// �o�^����Ă�������̏���ݒ�
	for (Light* light : lights)
	{
		light->PushRenderContext(rc);
	}
}

// �f�o�b�OGUI�`��
void LightManager::DrawDebugGUI()
{
	if (ImGui::TreeNode("Lights"))
	{
		ImGui::ColorEdit3("AmbientColor", &ambientColor.x);
		int nodeId = 0;
		for (Light* light : lights)
		{
			ImGui::PushID(nodeId++);
			light->DrawDebugGUI();
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
}

// �f�o�b�O�v���~�e�B�u�`��
void LightManager::DrawDebugPrimitive()
{
	for (Light* light : lights)
	{
		light->DrawDebugPrimitive();
	}
}
