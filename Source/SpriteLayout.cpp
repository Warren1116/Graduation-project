#include	"SpriteLayout.h"

int	SpriteLayout::ShiftJisToUnicode(const char* shiftJis, wchar_t* unicode, uint64_t unicodeBufSize)
{
	const	SIZE_T	strLen = strlen(shiftJis) + 1;
	return	MultiByteToWideChar(CP_ACP, 0, shiftJis, static_cast<int>(strLen) + 1, unicode, static_cast<int>(unicodeBufSize));
}

int	SpriteLayout::UnicodeToUTF8(const wchar_t* unicode, char* utf8, uint64_t utf8BufSize)
{
	const	SIZE_T	strLen = wcslen(unicode) + 1;
	return	WideCharToMultiByte(CP_UTF8, 0, unicode, static_cast<int>(strLen) + 1, utf8, static_cast<int>(utf8BufSize), NULL, NULL);
}

int	SpriteLayout::ShiftJisToUTF8(const char* shiftJis, char* utf8, uint64_t unicodeBufSize)
{
	wchar_t	strUnicode[MAX_PATH];
	if (ShiftJisToUnicode(shiftJis, strUnicode, MAX_PATH) > 0)
	{
		return	UnicodeToUTF8(strUnicode, utf8, unicodeBufSize);
	}
	return	0;
}

int	SpriteLayout::UTF8ToUnicode(const char* utf8, wchar_t* unicode, uint64_t unicodeBufSize)
{
	const	SIZE_T	strLen = strlen(utf8) + 1;
	return	MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(strLen) + 1, unicode, static_cast<int>(unicodeBufSize));
}

int	SpriteLayout::UnicodeToShiftJis(const wchar_t* unicode, char* shiftJis, uint64_t shiftJisBufSize)
{
	const	SIZE_T	strLen = wcslen(unicode) + 1;
	return	WideCharToMultiByte(CP_ACP, 0, unicode, static_cast<int>(strLen) + 1, shiftJis, static_cast<int>(shiftJisBufSize), NULL, NULL);
}

int	SpriteLayout::UTF8ToShiftJis(const char* utf8, char* shiftJis, uint64_t shiftJisBufSize)
{
	wchar_t	strUnicode[MAX_PATH];
	if (UTF8ToUnicode(utf8, strUnicode, MAX_PATH) > 0)
	{
		return	UnicodeToShiftJis(strUnicode, shiftJis, shiftJisBufSize);
	}
	return	0;
}

// レイアウト調整して表示
void SpriteLayout::Draw(ID3D11DeviceContext* dc, DisplayInfo info, DirectX::XMFLOAT4 color)
{
	float	textureWidth = static_cast<float>(info.sprite->GetTextureWidth());
	float	textureHeight = static_cast<float>(info.sprite->GetTextureHeight());
	//	コーナーサイズ規定
	float	width = textureWidth * info.uvSplitSize * info.cornerSizeRate.x;
	float	height = textureHeight * info.uvSplitSize * info.cornerSizeRate.y;
	if (info.size.x < width * 2)
		width = info.size.x / 2;
	if (info.size.y < height * 2)
		height = info.size.y / 2;
	//	座標・UV値算出
	const	DirectX::XMFLOAT2	Positions[4][4] =
	{
		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y - info.size.y + 0.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y - info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y - info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y - info.size.y + 0.0f}
		},
		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y - info.size.y + height * 1.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y - info.size.y + height * 1.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y - info.size.y + height * 1.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y - info.size.y + height * 1.0f}
		},
		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y + info.size.y - height * 1.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y + info.size.y - height * 1.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y + info.size.y - height * 1.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y + info.size.y - height * 1.0f}
		},
		{
			{info.center.x - info.size.x + width * 0.0f, info.center.y + info.size.y + 0.0f},
			{info.center.x - info.size.x + width * 1.0f, info.center.y + info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 1.0f, info.center.y + info.size.y + 0.0f},
			{info.center.x + info.size.x - width * 0.0f, info.center.y + info.size.y + 0.0f}
		},
	};
	const	DirectX::XMFLOAT2	UVs[4][4] =
	{
		{
			{                   0.0f,                    0.0f},
			{0.0f + info.uvSplitSize,                    0.0f},
			{1.0f - info.uvSplitSize,                    0.0f},
			{                   1.0f,                    0.0f}
		},
		{
			{                   0.0f, 0.0f + info.uvSplitSize},
			{0.0f + info.uvSplitSize, 0.0f + info.uvSplitSize},
			{1.0f - info.uvSplitSize, 0.0f + info.uvSplitSize},
			{                   1.0f, 0.0f + info.uvSplitSize}
		},
		{
			{                   0.0f, 1.0f - info.uvSplitSize},
			{0.0f + info.uvSplitSize, 1.0f - info.uvSplitSize},
			{1.0f - info.uvSplitSize, 1.0f - info.uvSplitSize},
			{                   1.0f, 1.0f - info.uvSplitSize}
		},
		{
			{                   0.0f,                    1.0f},
			{0.0f + info.uvSplitSize,                    1.0f},
			{1.0f - info.uvSplitSize,                    1.0f},
			{                   1.0f,                    1.0f}
		}
	};

	//	ウィンドウ描画
	for (int xx = 0; xx < 3; ++xx)
	{
		for (int yy = 0; yy < 3; ++yy)
		{
			info.sprite->Render(dc,
				info.center.x + (Positions[yy + 0][xx + 0].x - info.center.x),
				info.center.y + (Positions[yy + 0][xx + 0].y - info.center.y),
				(Positions[yy + 1][xx + 1].x - Positions[yy + 0][xx + 0].x),
				(Positions[yy + 1][xx + 1].y - Positions[yy + 0][xx + 0].y),

				textureWidth * UVs[yy + 0][xx + 0].x,
				textureHeight * UVs[yy + 0][xx + 0].y,
				textureWidth * (UVs[yy + 1][xx + 1].x - UVs[yy + 0][xx + 0].x),
				textureHeight * (UVs[yy + 1][xx + 1].y - UVs[yy + 0][xx + 0].y),
				0, Sprite::Pivot::CenterCenter,
				color.x, color.y, color.z, color.w);
		}
	}
}

// レイアウト調整後の表示矩形を算出
DirectX::XMFLOAT4 SpriteLayout::CalcDisplayRect(DisplayInfo info)
{
	float	textureWidth = static_cast<float>(info.sprite->GetTextureWidth());
	float	textureHeight = static_cast<float>(info.sprite->GetTextureHeight());
	float	width = textureWidth * info.uvSplitSize * info.cornerSizeRate.x;
	float	height = textureHeight * info.uvSplitSize * info.cornerSizeRate.y;
	if (info.size.x < width * 2)
		width = info.size.x / 2;
	if (info.size.y < height * 2)
		height = info.size.y / 2;
	float	l = info.center.x - info.size.x + width * 1.0f;
	float	t = info.center.y - info.size.y + height * 1.0f;
	float	r = info.center.x + info.size.x - width * 1.0f;
	float	b = info.center.y + info.size.y - height * 1.0f;
	return	{ l, t, r - l, b - t };
}
