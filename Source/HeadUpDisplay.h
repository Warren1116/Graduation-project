#pragma once

#include	"Graphics/Sprite.h"
#include	"Graphics/Font.h"
#include	<map>
#include	<vector>
#include	<string>
#include	"MessageData.h"

//	��ʕ\�����
class	HeadUpDisplay
{
	static	constexpr	float	WindowSplitSize = 0.3333f;

	// �����o���A�j���[�V��������
	static	constexpr	float	SpeechBalloonsDataAnimationTime = 0.1f;
	// ���b�Z�[�W�E�B���h�E�A�j���[�V��������
	static	constexpr	float	MessageWindowDataAnimationTime = 0.5;
	// �ʒm�E�B���h�E�\������
	static	constexpr	float	NotificationAnimationTime = 3.0f;
	// ���l�|�b�v�A�b�v�\������
	static	constexpr	float	PopupNumberAnimationTime = 1.0f;

	struct TalkPicture
	{
		int						index;
		TalkPicturePosition		position;
		bool					xflip;
	};

	using	Pictures = std::map<int, std::unique_ptr<Sprite>>;
	using	PictureData = std::map<int, TalkPicture>;

public:
	HeadUpDisplay();
	~HeadUpDisplay();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc);

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �N���A
	void Clear();

private:
	// ���b�Z�[�W�󂯎��
	void OnMessageEvent(void* data);

	// ���b�Z�[�W����
	void RenderMessageEvent(ID3D11DeviceContext* dc);

	// �����G���\���󂯎��
	void OnDisplayTalkPicture(void* data);

	// �����G����\���󂯎��
	void OnHideTalkPicture(void* data);

	// �����G�ǂݍ��ݎ󂯎��
	void OnLoadTalkPicture(void* data);

	// �����G�j���󂯎��
	void OnDisposeTalkPicture(void* data);

	// �����G�`��
	void RenderTalkPicture(ID3D11DeviceContext* dc);

	// �X�`���`��
	// void RenderStill(ID3D11DeviceContext* dc);

private:
	Font* font = nullptr;
	Sprite* messageWindow = nullptr;
	Sprite* notificationWindow = nullptr;

	//	�����G���
	uint64_t	DISPLAYTALKPICTUREINDEX = -1;
	uint64_t	HIDETALKPICTUREINDEX = -1;
	uint64_t	LOADTALKPICTUREINDEX = -1;
	uint64_t	DISPOSETALKPICTUREINDEX = -1;
	PictureData	talkPictureData;
	Pictures	talkPictures;

	//	���b�Z�[�W�E�B���h�E
	DirectX::XMFLOAT2	windowCornerSizeRate = { 0.5f, 0.5f };
	uint64_t			MESSAGEEVENTINDEX = -1;
	float				windowMessageElapsedTime = 0;
	std::string			windowMessageText = "";

	// IMGUI
	int debug_DisplayTalkId = 0;
	int debug_DisplayTalkPictureid = 0;
	TalkPicturePosition debug_DisplayTalkPicturePosition;
	bool debug_DisplayTalkPictureDirection;
	int debug_DisposeTalkPictureid = 0;
	int debug_TalkPictureid = 0;
	char debug_TalkPictureFilepath[1024];
};
