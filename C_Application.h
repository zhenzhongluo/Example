#ifndef TEST_C_APPLICATION_H
#define TEST_C_APPLICATION_H

#include "box.h"
#include "Cannon.h"

class C_Application
{
public:
	
	typedef unsigned int T_PressedKey;

	C_Application(int screenWidth, int screenHeight);
	~C_Application();

	/// Tick is called on fix framerate (50fps)
	void Tick(T_PressedKey pressedKeys);

	static const T_PressedKey s_KeyLeft  = 0x01;
	static const T_PressedKey s_KeyUp    = 0x02;
	static const T_PressedKey s_KeyRight = 0x04;
	static const T_PressedKey s_KeyDown  = 0x08;
	static const T_PressedKey s_KeySpace = 0x10;

	static const float k_PI;
private:

	void SpawnBox(int boxIndex, float x, float y);

	void OnCollision();
	void OnCannonBoxCollisionDetection(int boxListSize);
	void OnBoxCollisionDetection(int boxListSize);
	void OnBoxProjectileCollisionDetection(Projectile** projectileList
										 , int projectileListSize
										 , int boxListSize);
	void OnResetGame();
	
	const int	m_ScreenWidth;
	const int	m_ScreenHeight;

	int			m_boxCounter;
	
	Cannon		m_Cannon;
	Box			m_Boxes[3];
};

#endif // #ifndef TEST_C_APPLICATION_H
