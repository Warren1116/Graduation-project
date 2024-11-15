#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>

// イベントスクリプト実行者
// 良いサンプルとしては吉里吉里とかいいと思う(使ったことないけど……)
class EventScripter
{
	// 命令タイプ
	enum class CommandType
	{
		WINDOW_MESSAGE = 0,		// ウィンドウメッセージ表示
		INPUT_KEY_WAIT,			// キー入力待機
		TIME_WAIT,				// 時間待機

		DISPLAY_TALKPICTURE,	// 立ち絵表示
		HIDE_TALKPICTURE,		// 立ち絵非表示
		LOAD_TALKPICTURE,		// 立ち絵読み込み
		DISPOSE_TALKPICTURE,	// 立ち絵破棄

		PLAYER_SETPOSITION,		// プレイヤーの位置設定
		PLAYER_MOVE,			// プレイヤーの移動
		PLAYER_MOTION,			// プレイヤーのモーション

		CAMERA_SETPOSITION,		// プレイヤーの位置設定
		CAMERA_SETTARGET,		// プレイヤーのターゲット位置設定
	};

	// 命令データ
	struct CommandData
	{
		CommandType type; // 命令タイプ
		union Data
		{
			// メッセージ
			struct
			{
				char text[1024];
			} message;

			// 経過時間待機
			struct
			{
				float second;
			} time_wait;

			///////////////////////////////////////////
			// 立ち絵表示
			struct
			{
				int		id;
				int		displayPictureId;
				int		position;
				bool	xflip;
			} displayTalkPicture;
			// 立ち絵非表示
			struct
			{
				int		id;
			} hideTalkPicture;
			// 立ち絵読み込み
			struct
			{
				int		id;
				char	filepath[1024];
			} loadTalkPicture;
			// 立ち絵破棄
			struct
			{
				int			id;
			} disposeTalkPicture;

			///////////////////////////////////////////
			// プレイヤーの位置設定
			struct
			{
				DirectX::XMFLOAT3 position;
			} playerSetPosition;
			// プレイヤーの移動
			struct
			{
				DirectX::XMFLOAT3 moveVec;
				float moveSpeed;
				float turnSpeed;
			} playerMove;
			// プレイヤーのモーション
			struct
			{
				int index;
				bool loop;
				float blendSecond;
			} playerMotion;

			///////////////////////////////////////////
			// カメラの位置設定
			struct
			{
				DirectX::XMFLOAT3 position;
			} cameraSetPosition;
			// カメラのターゲット設定
			struct
			{
				DirectX::XMFLOAT3 position;
			} cameraTarget;
		} data;
	};

private:
	EventScripter() {}
	~EventScripter() {}

public:
	// 唯一のインスタンス取得
	static EventScripter& Instance()
	{
		static EventScripter instance;
		return instance;
	}

	void Initialize();

	void Finalize();

	void Update(float elapsedTime);

	void LoadScript(const char* filePath);

private:
	void OnEventMode(void* data);

private:
	std::vector<CommandData>	commands;
	int							commandIndex;
	float						timeWaitTimer = 0;

	// メッセンジャー識別子
	uint64_t	EVENTMODEINDEX = -1;
};

