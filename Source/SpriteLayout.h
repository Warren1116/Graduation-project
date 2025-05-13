//#pragma once
//
//#include	"Graphics/Sprite.h"
//#include	"Graphics/Font.h"
//#include	<vector>
//#include	<string>
//
//namespace	SpriteLayout
//{
//	// 文字コードの変換処理系関数
//	int	ShiftJisToUnicode(const char* shiftJis, wchar_t* unicode, uint64_t unicodeBufSize);
//	int	UnicodeToUTF8(const wchar_t* unicode, char* utf8, uint64_t utf8BufSize);
//	int	ShiftJisToUTF8(const char* shiftJis, char* utf8, uint64_t unicodeBufSize);
//	int	UTF8ToUnicode(const char* utf8, wchar_t* unicode, uint64_t unicodeBufSize);
//	int	UnicodeToShiftJis(const wchar_t* unicode, char* shiftJis, uint64_t shiftJisBufSize);
//	int	UTF8ToShiftJis(const char* utf8, char* shiftJis, uint64_t shiftJisBufSize);
//
//	// テクスチャの縦横を3分割して表示する
//	// ┏━┳━┳━┓
//	// ┃１┃２┃３┃
//	// ┣━╋━╋━┫
//	// ┃４┃５┃６┃
//	// ┣━╋━╋━┫
//	// ┃７┃８┃９┃
//	// ┗━┻━┻━┛
//	// 【１・３・７・９】のサイズをテクスチャで指定した一定の値に規定しつつ、
//	// 【２・４・５・６・８】のサイズを伸縮させて表示させる
//	struct	DisplayInfo
//	{
//		Sprite* sprite;
//		DirectX::XMFLOAT2	center;
//		DirectX::XMFLOAT2	size;
//		DirectX::XMFLOAT2	cornerSizeRate;
//		float				uvSplitSize;
//	};
//
//	// レイアウト調整して表示。↑のブロックの定義どおり
//	void Draw(ID3D11DeviceContext* dc, DisplayInfo info, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 });
//
//	// レイアウト調整後の表示矩形を算出。↑の【５】の矩形を算出する
//	DirectX::XMFLOAT4 CalcDisplayRect(DisplayInfo info);
//};
