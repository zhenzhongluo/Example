#pragma once

#include "ICollision.h"
#include "Player.h"
#include "Projectile.h"

class Cannon : public Player,
			   public ICollision
{
public:
	Cannon(int screenWidth, int screenHeight);
	~Cannon();

	// Game Object class
	void Tick(const int screenWidth, const int screenHeight) override;
	void OnDraw() const override;
	void OnDestroySelf() override;

	// Player class
	void OnTranslate(const unsigned int keyType, float speed) override;
	void OnRotate(const unsigned int keyType, float angleOffset) override;

	// ICollision class
	bool IsOverlapped(GameObject* target) override;

	void OnAttack(bool isAttacking);

	Projectile** GetProjectileList() { return m_projectileList; }
	int GetProjectileListSize() { return m_projectileListSize; }

protected:
	// Player class
	void UpdateDirection() override;

private:
	// Rotation
	void UpdateAngleInDegree(const unsigned int keyType, float angleOffset);
	void UpdateRotationMatrix();

	void UpdateTriangleCenterPoint();
	void InitProjectiles();
	
	float m_angleInDegree;

	float m_DeltaTime;

	int m_projectileIndex;

	unsigned int m_lastTranslationKeyPressed;
	unsigned int m_lastRotationKeyPressed;

	size_t m_projectileListSize;

	Vector m_pointA;
	Vector m_pointB;
	Vector m_pointC;

	// This is the center point of the Cannon (triangle); it's for Center of Gravity.
	Vector m_triangleCenterPoint;

	Projectile* m_projectileList[15];

	Matrix m_rotationMatrix;
	
};