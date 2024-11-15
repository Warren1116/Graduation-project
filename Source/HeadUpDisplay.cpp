#include	"HeadUpDisplay.h"

#include	<string.h>
#include	"MessageData.h"
#include	"Messenger.h"

#include	"Camera.h"
#include	"Graphics/Graphics.h"

#include	"SpriteLayout.h"

HeadUpDisplay::HeadUpDisplay()
{
	font = new Font("Data/Font/MS Gothic.fnt", 1024 * 16);
	messageWindow = new Sprite("Data/Sprite/MessageWindow.png");
	notificationWindow = new Sprite("Data/Sprite/NotificationWindow.png");
	DISPLAYTALKPICTUREINDEX = Messenger::Instance().AddReceiver(MessageData::DISPLAYTALKPICTUREEVENT, [&](void* data) { OnDisplayTalkPicture(data); });
	HIDETALKPICTUREINDEX = Messenger::Instance().AddReceiver(MessageData::HIDETALKPICTUREEVENT, [&](void* data) { OnHideTalkPicture(data); });

	LOADTALKPICTUREINDEX = Messenger::Instance().AddReceiver(MessageData::LOADTALKPICTUREEVENT, [&](void* data) { OnLoadTalkPicture(data); });
	DISPOSETALKPICTUREINDEX = Messenger::Instance().AddReceiver(MessageData::DISPOSETALKPICTUREEVENT, [&](void* data) { OnDisposeTalkPicture(data); });

	MESSAGEEVENTINDEX = Messenger::Instance().AddReceiver(MessageData::MESSAGEEVENT, [&](void* data) { OnMessageEvent(data); });

	windowMessageText.resize(1024);
}

HeadUpDisplay::~HeadUpDisplay()
{
	if (font != nullptr)
	{
		delete	font;
		font = nullptr;
	}
	if (messageWindow != nullptr)
	{
		delete	messageWindow;
		messageWindow = nullptr;
	}
	if (notificationWindow != nullptr)
	{
		delete	notificationWindow;
		notificationWindow = nullptr;
	}
	Messenger::Instance().RemoveReceiver(DISPLAYTALKPICTUREINDEX);
	Messenger::Instance().RemoveReceiver(HIDETALKPICTUREINDEX);

	Messenger::Instance().RemoveReceiver(LOADTALKPICTUREINDEX);
	Messenger::Instance().RemoveReceiver(DISPOSETALKPICTUREINDEX);

	Messenger::Instance().RemoveReceiver(MESSAGEEVENTINDEX);
}

// �X�V����
void HeadUpDisplay::Update(float elapsedTime)
{
	//	�E�B���h�E���b�Z�[�W�̍X�V
	{
		//	���ߒl�Z�o
		windowMessageElapsedTime += elapsedTime;
		if (windowMessageElapsedTime >= MessageWindowDataAnimationTime)
			windowMessageElapsedTime = MessageWindowDataAnimationTime;
	}
}

// �`�揈��
void HeadUpDisplay::Render(ID3D11DeviceContext* dc)
{
	// �t�H���g�X�^�b�N�J�n
	font->Begin(dc);

	// RenderStill(dc);

	// �����G�\��
	RenderTalkPicture(dc);

	// �E�B���h�E���b�Z�[�W�\��
	RenderMessageEvent(dc);

	// �t�H���g�`��
	font->End(dc);
}

// �f�o�b�O�pGUI�`��
void HeadUpDisplay::DrawDebugGUI()
{
	if (ImGui::Begin("Talk Event", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputInt("Display ID", &debug_DisplayTalkId);
			ImGui::InputInt("Display ImageID", &debug_DisplayTalkPictureid);
			ImGui::SliderInt("Display Position", &static_cast<int>(debug_DisplayTalkPicturePosition), 0, 2);
			ImGui::Checkbox("Display Direction", &debug_DisplayTalkPictureDirection);
			if (ImGui::Button("Add"))
			{
				MessageData::DISPLAYTALKPICTUREDATA	data;
				data.id = debug_DisplayTalkId;
				data.displayPictureId = debug_DisplayTalkPictureid;
				data.position = debug_DisplayTalkPicturePosition;
				data.xflip = debug_DisplayTalkPictureDirection;
				OnDisplayTalkPicture(&data);
			}
			ImGui::InputInt("Hide ImageID", &debug_DisposeTalkPictureid);
			if (ImGui::Button("Dispose"))
			{
				MessageData::HIDETALKPICTUREDATA	data;
				data.id = debug_DisposeTalkPictureid;
				OnHideTalkPicture(&data);
			}
			// ���X�g�\��
			int iiii = 0;
			for (auto& it : talkPictureData)
			{
				ImGui::PushID(it.first);
				ImGui::Text("Display ImageID %d", it.second.index);
				ImGui::Text("Display Position %d", it.second.position);
				ImGui::Text("Display Direction %d", it.second.xflip);
				ImGui::PopID();
			}
		}
		// �����G�֌W
		if (ImGui::CollapsingHeader("TalkPictures", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::CollapsingHeader("Load...", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::InputInt("ID", &debug_TalkPictureid);
				ImGui::InputText("FilePath", debug_TalkPictureFilepath, sizeof(debug_TalkPictureFilepath));
				if (ImGui::Button("Load"))
				{
					MessageData::LOADTALKPICTUREDATA	data;
					data.id = debug_TalkPictureid;
					data.filepath = debug_TalkPictureFilepath;
					OnLoadTalkPicture(&data);
				}
				if (ImGui::Button("Dispose"))
				{
					MessageData::DISPOSETALKPICTUREDATA	data;
					data.id = debug_TalkPictureid;
					OnDisposeTalkPicture(&data);
				}
			}
			// ���X�g�\��
			for (auto& it : talkPictures)
			{
				ImGui::Text("%d", it.first);
				ImGui::Image((void*)(intptr_t)it.second->GetShaderResourceView().Get(), { 300, 300 });
			}
		}
	}
	ImGui::End();
}

// �N���A
void HeadUpDisplay::Clear()
{
	talkPictureData.clear();
	talkPictures.clear();
}

// ���b�Z�[�W�󂯎��
void HeadUpDisplay::OnMessageEvent(void* data)
{
	MessageData::MESSAGEEVENTDATA* p = static_cast<MessageData::MESSAGEEVENTDATA*>(data);
	// ���b�Z�[�W
	windowMessageElapsedTime = 0;
	windowMessageText = p->message;
}

// ���b�Z�[�W����
void HeadUpDisplay::RenderMessageEvent(ID3D11DeviceContext* dc)
{
	if (windowMessageText.empty())
		return;

	// �A�j���[�V�����K���l
	float value = windowMessageElapsedTime / MessageWindowDataAnimationTime;

	// �\���E�B���h�E��`�v�Z
	const float sw = Graphics::Instance().GetScreenWidth();
	const float sh = Graphics::Instance().GetScreenHeight();
	DirectX::XMFLOAT2 size = { sw * 0.5f * value, sh * 0.15f * value };
	DirectX::XMFLOAT2 center = { sw * 0.5f, sh * 0.85f };

	SpriteLayout::DisplayInfo displayInfo = { messageWindow, center, size, windowCornerSizeRate, WindowSplitSize };

	// �E�B���h�E�\��
	SpriteLayout::Draw(dc, displayInfo);
	if (value < 0.999f)
		return;

	// ���b�Z�[�W�\��
	DirectX::XMFLOAT4 displayRect = SpriteLayout::CalcDisplayRect(displayInfo);
	font->Draw(displayRect.x, displayRect.y, windowMessageText);
}

// ���b�Z�[�W����
void HeadUpDisplay::RenderTalkPicture(ID3D11DeviceContext* dc)
{
	const float AxisWidth = Graphics::Instance().GetScreenWidth() / 4;
	const float AxisHeight = Graphics::Instance().GetScreenHeight() / 4;
	const float PeaceWidth = Graphics::Instance().GetScreenWidth() / 3;
	const float PeaceHeight = Graphics::Instance().GetScreenHeight();
	const float positions[] =
	{
		AxisWidth * 1 - PeaceWidth / 2, 0, PeaceWidth, PeaceHeight,	//	Left
		AxisWidth * 2 - PeaceWidth / 2, 0, PeaceWidth, PeaceHeight,	//	Center
		AxisWidth * 3 - PeaceWidth / 2, 0, PeaceWidth, PeaceHeight,	//	Right
	};
	for (auto& data : talkPictureData)
	{
		int positionId = static_cast<int>(data.second.position) * 4;
		Pictures::iterator	it = talkPictures.find(data.second.index);
		if (it == talkPictures.end())
			continue;

		std::unique_ptr<Sprite>& pic = it->second;
		float texLeft = 0;
		float texWidth = static_cast<float>(pic->GetTextureWidth());
		if (data.second.xflip)
		{
			texWidth = -texWidth;
		}
		pic->Render(dc,
			positions[positionId + 0], positions[positionId + 1],
			positions[positionId + 2], positions[positionId + 3],
			texLeft, 0,
			texWidth, static_cast<float>(pic->GetTextureHeight()),
			0, Sprite::Pivot::LeftTop,
			1, 1, 1, 1);
	}
}

// �����G���\���󂯎��
void HeadUpDisplay::OnDisplayTalkPicture(void* data)
{
	MessageData::DISPLAYTALKPICTUREDATA* p = static_cast<MessageData::DISPLAYTALKPICTUREDATA*>(data);
	PictureData::iterator	it = talkPictureData.find(p->id);
	if (it != talkPictureData.end())
		talkPictureData.erase(it);

	TalkPicture	talkPicture = { p->displayPictureId, p->position, p->xflip };
	talkPictureData.insert(std::make_pair(p->id, talkPicture));
}

// �����G����\���󂯎��
void HeadUpDisplay::OnHideTalkPicture(void* data)
{
	MessageData::HIDETALKPICTUREDATA* p = static_cast<MessageData::HIDETALKPICTUREDATA*>(data);
	PictureData::iterator	it = talkPictureData.find(p->id);
	if (it == talkPictureData.end())
		return;
	talkPictureData.erase(it);
}

// �����G�ǂݍ��ݎ󂯎��
void HeadUpDisplay::OnLoadTalkPicture(void* data)
{
	MessageData::LOADTALKPICTUREDATA* p = static_cast<MessageData::LOADTALKPICTUREDATA*>(data);
	Pictures::iterator	it = talkPictures.find(p->id);
	if (it != talkPictures.end())
		talkPictures.erase(it);

	talkPictures.insert(std::make_pair(p->id, new Sprite(p->filepath.data())));
}

// �����G�ǂݍ��ݎ󂯎��
void HeadUpDisplay::OnDisposeTalkPicture(void* data)
{
	MessageData::DISPOSETALKPICTUREDATA* p = static_cast<MessageData::DISPOSETALKPICTUREDATA*>(data);
	Pictures::iterator	it = talkPictures.find(p->id);
	if (it == talkPictures.end())
		return;
	talkPictures.erase(it);
}


