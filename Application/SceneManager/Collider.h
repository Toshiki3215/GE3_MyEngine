// Collider.h
// ƒQ[ƒ€ƒV[ƒ“‚Ì“–‚½‚è”»’è‚ğŠÇ—‚µ‚Ä‚¢‚é

#pragma once

class Collider
{
private:

	//Õ“Ë”¼Œa
	float radius = 10;

public:

	//”¼Œa‚ğæ“¾
	float GetRadius() { return radius; }

	// ”¼Œa‚ğİ’è
	void SetRadius(float r) { radius = r; }

	//Õ“Ë‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void OnCollision();

};