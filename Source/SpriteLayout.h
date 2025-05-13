//#pragma once
//
//#include	"Graphics/Sprite.h"
//#include	"Graphics/Font.h"
//#include	<vector>
//#include	<string>
//
//namespace	SpriteLayout
//{
//	// �����R�[�h�̕ϊ������n�֐�
//	int	ShiftJisToUnicode(const char* shiftJis, wchar_t* unicode, uint64_t unicodeBufSize);
//	int	UnicodeToUTF8(const wchar_t* unicode, char* utf8, uint64_t utf8BufSize);
//	int	ShiftJisToUTF8(const char* shiftJis, char* utf8, uint64_t unicodeBufSize);
//	int	UTF8ToUnicode(const char* utf8, wchar_t* unicode, uint64_t unicodeBufSize);
//	int	UnicodeToShiftJis(const wchar_t* unicode, char* shiftJis, uint64_t shiftJisBufSize);
//	int	UTF8ToShiftJis(const char* utf8, char* shiftJis, uint64_t shiftJisBufSize);
//
//	// �e�N�X�`���̏c����3�������ĕ\������
//	// ��������������
//	// ���P���Q���R��
//	// ��������������
//	// ���S���T���U��
//	// ��������������
//	// ���V���W���X��
//	// ��������������
//	// �y�P�E�R�E�V�E�X�z�̃T�C�Y���e�N�X�`���Ŏw�肵�����̒l�ɋK�肵�A
//	// �y�Q�E�S�E�T�E�U�E�W�z�̃T�C�Y��L�k�����ĕ\��������
//	struct	DisplayInfo
//	{
//		Sprite* sprite;
//		DirectX::XMFLOAT2	center;
//		DirectX::XMFLOAT2	size;
//		DirectX::XMFLOAT2	cornerSizeRate;
//		float				uvSplitSize;
//	};
//
//	// ���C�A�E�g�������ĕ\���B���̃u���b�N�̒�`�ǂ���
//	void Draw(ID3D11DeviceContext* dc, DisplayInfo info, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 });
//
//	// ���C�A�E�g������̕\����`���Z�o�B���́y�T�z�̋�`���Z�o����
//	DirectX::XMFLOAT4 CalcDisplayRect(DisplayInfo info);
//};
