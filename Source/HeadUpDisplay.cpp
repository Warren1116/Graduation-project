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

// 更新処理
void HeadUpDisplay::Update(float elapsedTime)
{
	//	ウィンドウメッセージの更新
	{
		//	透過値算出
		windowMessageElapsedTime += elapsedTime;
		if (windowMessageElapsedTime >= MessageWindowDataAnimationTime)
			windowMessageElapsedTime = MessageWindowDataAnimationTime;
	}
}

// 描画処理
void HeadUpDisplay::Render(ID3D11DeviceContext* dc)
{
	// フォントスタック開始
	font->Begin(dc);

	// RenderStill(dc);

	// 立ち絵表示
	RenderTalkPicture(dc);

	// ウィンドウメッセージ表示
	RenderMessageEvent(dc);

	// フォント描画
	font->End(dc);
}

// デバッグ用GUI描画
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
			// リスト表示
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
		// 立ち絵関係
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
			// リスト表示
			for (auto& it : talkPictures)
			{
				ImGui::Text("%d", it.first);
				ImGui::Image((void*)(intptr_t)it.second->GetShaderResourceView().Get(), { 300, 300 });
			}
		}
	}
	ImGui::End();
}

// クリア
void HeadUpDisplay::Clear()
{
	talkPictureData.clear();
	talkPictures.clear();
}

// メッセージ受け取り
void HeadUpDisplay::OnMessageEvent(void* data)
{
	MessageData::MESSAGEEVENTDATA* p = static_cast<MessageData::MESSAGEEVENTDATA*>(data);
	// メッセージ
	windowMessageElapsedTime = 0;
	windowMessageText = p->message;
}

// メッセージ処理
void HeadUpDisplay::RenderMessageEvent(ID3D11DeviceContext* dc)
{
	if (windowMessageText.empty())
		return;

	// アニメーション適応値
	float value = windowMessageElapsedTime / MessageWindowDataAnimationTime;

	// 表示ウィンドウ矩形計算
	const float sw = Graphics::Instance().GetScreenWidth();
	const float sh = Graphics::Instance().GetScreenHeight();
	DirectX::XMFLOAT2 size = { sw * 0.5f * value, sh * 0.15f * value };
	DirectX::XMFLOAT2 center = { sw * 0.5f, sh * 0.85f };

	SpriteLayout::DisplayInfo displayInfo = { messageWindow, center, size, windowCornerSizeRate, WindowSplitSize };

	// ウィンドウ表示
	SpriteLayout::Draw(dc, displayInfo);
	if (value < 0.999f)
		return;

	// メッセージ表示
	DirectX::XMFLOAT4 displayRect = SpriteLayout::CalcDisplayRect(displayInfo);
	font->Draw(displayRect.x, displayRect.y, windowMessageText);
}

// メッセージ処理
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

// 立ち絵情報表示受け取り
void HeadUpDisplay::OnDisplayTalkPicture(void* data)
{
	MessageData::DISPLAYTALKPICTUREDATA* p = static_cast<MessageData::DISPLAYTALKPICTUREDATA*>(data);
	PictureData::iterator	it = talkPictureData.find(p->id);
	if (it != talkPictureData.end())
		talkPictureData.erase(it);

	TalkPicture	talkPicture = { p->displayPictureId, p->position, p->xflip };
	talkPictureData.insert(std::make_pair(p->id, talkPicture));
}

// 立ち絵情報非表示受け取り
void HeadUpDisplay::OnHideTalkPicture(void* data)
{
	MessageData::HIDETALKPICTUREDATA* p = static_cast<MessageData::HIDETALKPICTUREDATA*>(data);
	PictureData::iterator	it = talkPictureData.find(p->id);
	if (it == talkPictureData.end())
		return;
	talkPictureData.erase(it);
}

// 立ち絵読み込み受け取り
void HeadUpDisplay::OnLoadTalkPicture(void* data)
{
	MessageData::LOADTALKPICTUREDATA* p = static_cast<MessageData::LOADTALKPICTUREDATA*>(data);
	Pictures::iterator	it = talkPictures.find(p->id);
	if (it != talkPictures.end())
		talkPictures.erase(it);

	talkPictures.insert(std::make_pair(p->id, new Sprite(p->filepath.data())));
}

// 立ち絵読み込み受け取り
void HeadUpDisplay::OnDisposeTalkPicture(void* data)
{
	MessageData::DISPOSETALKPICTUREDATA* p = static_cast<MessageData::DISPOSETALKPICTUREDATA*>(data);
	Pictures::iterator	it = talkPictures.find(p->id);
	if (it == talkPictures.end())
		return;
	talkPictures.erase(it);
}


