#include "C_Application.h"
#include "graphics.h"
#include "time.h"

#include "Cannon.h"
#include "Projectile.h"

const float C_Application::k_PI = 3.1415926536f;

C_Application::C_Application(int screenWidth, int screenHeight) : m_ScreenWidth(screenWidth)
																, m_ScreenHeight(screenHeight)
															    , m_Cannon(screenWidth, screenHeight)
																, m_boxCounter(0)
{
	SpawnBox(0, m_ScreenWidth / 2.f, m_ScreenHeight / 4.f);
	SpawnBox(1, m_ScreenWidth / 4.f, m_ScreenHeight / 2.f);
	SpawnBox(2, m_ScreenWidth * 3.f / 4.f, m_ScreenHeight / 2.f);
}

C_Application::~C_Application()
{

}


void C_Application::Tick(T_PressedKey pressedKeys)
{
	// Sample tick

	// Clear screen on cannon position

	FillRect(0, 0, m_ScreenWidth, m_ScreenHeight, GetRGB(0, 0, 0));

	// Key processing
	if (pressedKeys & s_KeySpace)
	{
		m_Cannon.OnAttack(true);
	}
	else
	{
		m_Cannon.OnAttack(false);
	}

	if(pressedKeys & s_KeyLeft)
	{
		m_Cannon.OnRotate(s_KeyLeft, 0.1f);
	}

	if(pressedKeys & s_KeyRight)
	{
		m_Cannon.OnRotate(s_KeyRight, 0.1f);
	}

	if(pressedKeys & s_KeyUp)
	{
		m_Cannon.OnTranslate(s_KeyUp, 4.0f);
	}

	if(pressedKeys & s_KeyDown)
	{
		m_Cannon.OnTranslate(s_KeyDown, 4.0f);
	}

	// Draw cannon
	m_Cannon.Tick(m_ScreenWidth, m_ScreenHeight);

	// Draw boxes
	for (int boxIndex = 0; boxIndex < 3; ++boxIndex)
	{
		Box &box = m_Boxes[boxIndex];
		box.Tick(m_ScreenWidth, m_ScreenHeight);
		box.OnDraw();
	}

	OnCollision();

	OnResetGame();
}

void C_Application::SpawnBox(int boxIndex, float x, float y)
{
	Vector boxCenter(x, y, 1.f);
	Matrix trans = Matrix::TranslationMatrix(boxCenter);
	m_Boxes[boxIndex].Transform(trans);
}

void C_Application::OnCollision()
{
	Projectile** projectileList = m_Cannon.GetProjectileList();
	int projectileListSize = m_Cannon.GetProjectileListSize();

	size_t boxListSize = sizeof(m_Boxes) /
						 sizeof(m_Boxes[0]);

	OnCannonBoxCollisionDetection(boxListSize);
	OnBoxCollisionDetection(boxListSize);
	OnBoxProjectileCollisionDetection(projectileList, projectileListSize, boxListSize);
}

void C_Application::OnCannonBoxCollisionDetection(int boxListSize)
{
	for (int boxIndex = 0; boxIndex < boxListSize; ++boxIndex)
	{
		Box* currentBox = &m_Boxes[boxIndex];

		if (m_Cannon.IsOverlapped(currentBox))
		{
			m_Cannon.OnDestroySelf();
		}
	}
}

void C_Application::OnBoxCollisionDetection(int boxListSize)
{
	for (int boxIndex = 0; boxIndex < boxListSize; ++boxIndex)
	{
		Box* currentBox = &m_Boxes[boxIndex];

		for (int k = boxIndex + 1; k < 3; ++k)
		{
			Box* target = &m_Boxes[k];

			if (currentBox->IsOverlapped(target))
			{
				currentBox->UpdateVelocity(-1.0f);
				target->UpdateVelocity(-1.0f);
			}
		}
	}
}

/*
	6. Make sure there is a testing of collisions of
	   the boxes with the projectiles shot from the cannon.
	   When the boxes are shot down, they and the projectile disappear.
*/
void C_Application::OnBoxProjectileCollisionDetection(Projectile** projectileList
													, int projectileListSize
													, int boxListSize)
{
	for (int projectileIndex = 0;
		 projectileIndex < projectileListSize;
		 ++projectileIndex)
	{
		Projectile* projectile = projectileList[projectileIndex];

		for (int boxIndex = 0; boxIndex < (int)boxListSize; ++boxIndex)
		{
			Box* target = &m_Boxes[boxIndex];

			if (projectile->IsOverlapped(target))
			{
				projectile->OnDestroySelf();

				target->OnDestroySelf();

				++m_boxCounter;
			}
		}
	}
}

/*
	7. Once the last boxes are shot down,
	   the three big boxes should appear again and the whole procedure repeats.
*/
void C_Application::OnResetGame()
{
	if (m_boxCounter == 3)
	{
		size_t boxListSize = sizeof(m_Boxes) /
						 	 sizeof(m_Boxes[0]);

		// Reset every box's localToWorld matrix because
		// after it's destroyed, this matrix within are all 0.
		for (int boxIndex = 0; boxIndex < boxListSize; ++boxIndex)
		{
			Box& box = m_Boxes[boxIndex];
			box.OnInitializeLocalToWorldMatrix();
		}

		SpawnBox(0, m_ScreenWidth / 2.f, m_ScreenHeight / 4.f);
		SpawnBox(1, m_ScreenWidth / 4.f, m_ScreenHeight / 2.f);
		SpawnBox(2, m_ScreenWidth * 3.f / 4.f, m_ScreenHeight / 2.f);

		// Reset counter.
		m_boxCounter = 0;
	}
}

