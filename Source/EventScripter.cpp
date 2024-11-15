#include <fstream>
#include <iostream>
#include <algorithm>

#include "Misc.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "EventScripter.h"

#include "MessageData.h"
#include "Messenger.h"

void EventScripter::Initialize()
{
	EVENTMODEINDEX = Messenger::Instance().AddReceiver(MessageData::EVENTMODEEVENT, [&](void* data) { OnEventMode(data); });
}

void EventScripter::Finalize()
{
	Messenger::Instance().RemoveReceiver(EVENTMODEINDEX);
}

void EventScripter::Update(float elapsedTime)
{
	if (commands.empty())
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();

	while (commandIndex != commands.size())
	{
		switch (commands[commandIndex].type)
		{
		case CommandType::WINDOW_MESSAGE:
		{
			MessageData::MESSAGEEVENTDATA data;
			data.message = commands[commandIndex].data.message.text;
			Messenger::Instance().SendData(MessageData::MESSAGEEVENT, &data);
			break;
		}
		case CommandType::INPUT_KEY_WAIT:
		{
			if (!(gamePad.GetButtonDown() & (GamePad::BTN_A | GamePad::BTN_B | GamePad::BTN_X | GamePad::BTN_Y)))
			{
				return;
			}
			++commandIndex;
			return;
		}
		case CommandType::TIME_WAIT:
		{
			timeWaitTimer += elapsedTime * 60;
			if (timeWaitTimer < commands[commandIndex].data.time_wait.second)
			{
				return;
			}
			++commandIndex;
			timeWaitTimer = 0;
			return;
		}
		//////////////////////////////////////////////////////////////////////////////
		case CommandType::DISPLAY_TALKPICTURE:
		{
			MessageData::DISPLAYTALKPICTUREDATA data;
			data.id = commands[commandIndex].data.displayTalkPicture.id;
			data.displayPictureId = commands[commandIndex].data.displayTalkPicture.displayPictureId;
			data.position = static_cast<TalkPicturePosition>(commands[commandIndex].data.displayTalkPicture.position);
			data.xflip = commands[commandIndex].data.displayTalkPicture.xflip;
			Messenger::Instance().SendData(MessageData::DISPLAYTALKPICTUREEVENT, &data);
			break;
		}
		case CommandType::HIDE_TALKPICTURE:
		{
			MessageData::HIDETALKPICTUREDATA data;
			data.id = commands[commandIndex].data.hideTalkPicture.id;
			Messenger::Instance().SendData(MessageData::HIDETALKPICTUREEVENT, &data);
			break;
		}
		case CommandType::LOAD_TALKPICTURE:
		{
			MessageData::LOADTALKPICTUREDATA data;
			data.id = commands[commandIndex].data.loadTalkPicture.id;
			data.filepath = commands[commandIndex].data.loadTalkPicture.filepath;
			Messenger::Instance().SendData(MessageData::LOADTALKPICTUREEVENT, &data);
			break;
		}
		case CommandType::DISPOSE_TALKPICTURE:
		{
			MessageData::DISPOSETALKPICTUREDATA data;
			data.id = commands[commandIndex].data.disposeTalkPicture.id;
			Messenger::Instance().SendData(MessageData::DISPOSETALKPICTUREEVENT, &data);
			break;
		}
		//////////////////////////////////////////////////////////////////////////////
		case CommandType::PLAYER_SETPOSITION:
		{
			MessageData::PLAYERSETPOSITIONDATA data;
			data.position = commands[commandIndex].data.playerSetPosition.position;
			Messenger::Instance().SendData(MessageData::PLAYERSETPOSITIONEVENT, &data);
			break;
		}
		case CommandType::PLAYER_MOVE:
		{
			MessageData::PLAYERMOVEDATA data;
			data.moveVec = commands[commandIndex].data.playerMove.moveVec;
			data.moveSpeed = commands[commandIndex].data.playerMove.moveSpeed;
			data.turnSpeed = commands[commandIndex].data.playerMove.turnSpeed;
			Messenger::Instance().SendData(MessageData::PLAYERMOVEEVENT, &data);
			break;
		}
		case CommandType::PLAYER_MOTION:
		{
			MessageData::PLAYERMOTIONDATA data;
			data.index = commands[commandIndex].data.playerMotion.index;
			data.loop = commands[commandIndex].data.playerMotion.loop;
			data.blendSecond = commands[commandIndex].data.playerMotion.blendSecond;
			Messenger::Instance().SendData(MessageData::PLAYERMOTIONEVENT, &data);
			break;
		}
		//////////////////////////////////////////////////////////////////////////////
		case CommandType::CAMERA_SETPOSITION:
		{
			MessageData::CAMERASETPOSITIONDATA data;
			data.position = commands[commandIndex].data.cameraSetPosition.position;
			Messenger::Instance().SendData(MessageData::CAMERASETPOSITIONEVENT, &data);
			break;
		}
		case CommandType::CAMERA_SETTARGET:
		{
			MessageData::CAMERASETTARGETDATA data;
			data.position = commands[commandIndex].data.cameraTarget.position;
			Messenger::Instance().SendData(MessageData::CAMERASETTARGETEVENT, &data);
			break;
		}
		}
		++commandIndex;
	}

	//	ここに来たらスクリプト終了なので色々と初期化
	commandIndex = 0;
	timeWaitTimer = 0;
	commands.clear();
	//	ゲームモードに移動
	MessageData::GAMEMODEDATA	data;
	Messenger::Instance().SendData(MessageData::GAMEMODEEVENT, &data);
}

void EventScripter::LoadScript(const char* filePath)
{
	// スクリプトファイルロード
	std::ifstream ifs(filePath);
	assert(!ifs.fail());
	if (ifs.fail())
		return;

	// 一旦初期化
	commandIndex = 0;
	timeWaitTimer = 0;
	commands.clear();

	// 構文解析
	std::string str;
	while (std::getline(ifs, str))
	{
		if (str.size() == 0)
			continue;

		// 文字列を分割して構文解析
		// 命令は全て1行で行い、パラメーターの分割は全て" "で行う
		// パラメーターの基本的な並びは以下の通り
		// 0 = 構文名, 1～ = データ
		std::vector<std::string> string_array = string_split(str, ' ');
		if (string_array.size() == 0)
			continue;

		if (string_array[0] == "WINDOW_MESSAGE")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::WINDOW_MESSAGE;
			std::string text;
			for (int i = 1; i < string_array.size(); ++i)
			{
				text += string_array[i];
				text += '\n';
			}
			sprintf_s(data.data.message.text, sizeof(data.data.message.text), text.data(), text.size());
		}
		else if (string_array[0] == "INPUT_KEY_WAIT")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::INPUT_KEY_WAIT;
		}
		else if (string_array[0] == "TIME_WAIT")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::TIME_WAIT;
			data.data.time_wait.second = static_cast<float>(std::atof(string_array[1].data())) * 60.0f;
		}
		//////////////////////////////////////////////////////////////////////////////
		else if (string_array[0] == "DISPLAY_TALKPICTURE")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::DISPLAY_TALKPICTURE;
			data.data.displayTalkPicture.id = std::atoi(string_array[1].data());
			data.data.displayTalkPicture.displayPictureId = std::atoi(string_array[2].data());
			data.data.displayTalkPicture.position = std::atoi(string_array[3].data());
			data.data.displayTalkPicture.xflip = std::atoi(string_array[4].data());
		}
		else if (string_array[0] == "HIDE_TALKPICTURE")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::HIDE_TALKPICTURE;
			data.data.hideTalkPicture.id = std::atoi(string_array[1].data());
		}
		else if (string_array[0] == "LOAD_TALKPICTURE")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::LOAD_TALKPICTURE;
			data.data.loadTalkPicture.id = std::atoi(string_array[1].data());
			sprintf_s(data.data.loadTalkPicture.filepath, sizeof(data.data.loadTalkPicture.filepath),
				string_array[2].data(), string_array[2].size());
		}
		else if (string_array[0] == "DISPOSE_TALKPICTURE")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::DISPOSE_TALKPICTURE;
			data.data.disposeTalkPicture.id = std::atoi(string_array[1].data());
		}
		//////////////////////////////////////////////////////////////////////////////
		else if (string_array[0] == "PLAYER_SETPOSITION")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::PLAYER_SETPOSITION;
			data.data.playerSetPosition.position.x = static_cast<float>(std::atof(string_array[1].data()));
			data.data.playerSetPosition.position.y = static_cast<float>(std::atof(string_array[2].data()));
			data.data.playerSetPosition.position.z = static_cast<float>(std::atof(string_array[3].data()));
		}
		else if (string_array[0] == "PLAYER_MOVE")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::PLAYER_MOVE;
			data.data.playerMove.moveVec.x = static_cast<float>(std::atof(string_array[1].data()));
			data.data.playerMove.moveVec.y = static_cast<float>(std::atof(string_array[2].data()));
			data.data.playerMove.moveVec.z = static_cast<float>(std::atof(string_array[3].data()));
			data.data.playerMove.moveSpeed = static_cast<float>(std::atof(string_array[4].data()));
			data.data.playerMove.turnSpeed = static_cast<float>(std::atof(string_array[5].data()));
		}
		else if (string_array[0] == "PLAYER_MOTION")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::PLAYER_MOTION;
			data.data.playerMotion.index = std::atoi(string_array[1].data());
			data.data.playerMotion.loop = std::atoi(string_array[2].data());
			data.data.playerMotion.blendSecond = static_cast<float>(std::atof(string_array[3].data()));
		}
		//////////////////////////////////////////////////////////////////////////////
		else if (string_array[0] == "CAMERA_SETPOSITION")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::CAMERA_SETPOSITION;
			data.data.cameraSetPosition.position.x = static_cast<float>(std::atof(string_array[1].data()));
			data.data.cameraSetPosition.position.y = static_cast<float>(std::atof(string_array[2].data()));
			data.data.cameraSetPosition.position.z = static_cast<float>(std::atof(string_array[3].data()));
		}
		else if (string_array[0] == "CAMERA_SETTARGET")
		{
			commands.emplace_back();
			CommandData& data = commands.back();
			data.type = CommandType::CAMERA_SETTARGET;
			data.data.cameraTarget.position.x = static_cast<float>(std::atof(string_array[1].data()));
			data.data.cameraTarget.position.y = static_cast<float>(std::atof(string_array[2].data()));
			data.data.cameraTarget.position.z = static_cast<float>(std::atof(string_array[3].data()));
		}
	}
	commandIndex = 0;
}

void EventScripter::OnEventMode(void* data)
{
	MessageData::EVENTMODEDATA* p = static_cast<MessageData::EVENTMODEDATA*>(data);
	LoadScript(p->filepath.data());
}
