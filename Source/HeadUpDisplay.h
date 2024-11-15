#pragma once

#include	"Graphics/Sprite.h"
#include	"Graphics/Font.h"
#include	<map>
#include	<vector>
#include	<string>
#include	"MessageData.h"

//	画面表示情報
class	HeadUpDisplay
{
	static	constexpr	float	WindowSplitSize = 0.3333f;

	// 吹き出しアニメーション時間
	static	constexpr	float	SpeechBalloonsDataAnimationTime = 0.1f;
	// メッセージウィンドウアニメーション時間
	static	constexpr	float	MessageWindowDataAnimationTime = 0.5;
	// 通知ウィンドウ表示時間
	static	constexpr	float	NotificationAnimationTime = 3.0f;
	// 数値ポップアップ表示時間
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

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc);

	// デバッグ用GUI描画
	void DrawDebugGUI();

	// クリア
	void Clear();

private:
	// メッセージ受け取り
	void OnMessageEvent(void* data);

	// メッセージ処理
	void RenderMessageEvent(ID3D11DeviceContext* dc);

	// 立ち絵情報表示受け取り
	void OnDisplayTalkPicture(void* data);

	// 立ち絵情報非表示受け取り
	void OnHideTalkPicture(void* data);

	// 立ち絵読み込み受け取り
	void OnLoadTalkPicture(void* data);

	// 立ち絵破棄受け取り
	void OnDisposeTalkPicture(void* data);

	// 立ち絵描画
	void RenderTalkPicture(ID3D11DeviceContext* dc);

	// スチル描画
	// void RenderStill(ID3D11DeviceContext* dc);

private:
	Font* font = nullptr;
	Sprite* messageWindow = nullptr;
	Sprite* notificationWindow = nullptr;

	//	立ち絵情報
	uint64_t	DISPLAYTALKPICTUREINDEX = -1;
	uint64_t	HIDETALKPICTUREINDEX = -1;
	uint64_t	LOADTALKPICTUREINDEX = -1;
	uint64_t	DISPOSETALKPICTUREINDEX = -1;
	PictureData	talkPictureData;
	Pictures	talkPictures;

	//	メッセージウィンドウ
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
