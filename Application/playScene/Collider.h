#pragma once

class Collider
{
private:

	//�Փ˔��a
	float radius = 10;

public:

	//���a���擾
	float GetRadius() { return radius; }

	// ���a��ݒ�
	void SetRadius(float r) { radius = r; }

	//�Փˎ��ɌĂ΂��֐�
	void OnCollision();

};