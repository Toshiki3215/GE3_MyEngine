#pragma once
#include <windows.h>
#include <wrl.h>
#include <dinput.h>
#include "WindowsApp.h"

class Input
{
public:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�

	// ������
	void Initialize(WinApp* winApp);

	// �X�V
	void Update();

	bool PushKey(BYTE keyNumber);

	//�g���K�[
	bool TriggerKey(BYTE keyNumber);

	//��������
	bool ReleaseKey(BYTE keyNumber);

private: // �����o�ϐ�

	// �L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> keyboard;

	// DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> directInput;

	// �S�L�[�̏��
	BYTE key[256] = {};

	// �O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	//windwsAPI
	WinApp* winApp_ = nullptr;
};