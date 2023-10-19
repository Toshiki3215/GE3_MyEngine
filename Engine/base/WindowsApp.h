// WindowsApp.h
// WindowsAPI���Ǘ����Ă�

#pragma once
#include <Windows.h>

class WinApp
{
public:

	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�Q�b�^�[(HWND)
	HWND GetHwnd() const { return hwnd; }

	//�Q�b�^�[(HINSTANCE)
	HINSTANCE GetHInstance() const { return w.hInstance; }

	//�v���Z�X���b�Z�[�W
	bool ProcessMessage();

	// �E�B���h�E����
	static const int window_width = 1280;

	// �E�B���h�E�c��
	static const int window_height = 720;

public: //�����o�֐�
	//������
	void Initialize();

	//�X�V
	void Update();

	//�I��
	void Finalize();
private:
	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;

	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};

};