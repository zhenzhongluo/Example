#pragma once

#include "ConsumableObject.h"
#include "ICollision.h"

class Cannon;

class Projectile : public ConsumableObject
				 , public ICollision
{
public:
	Projectile();
	Projectile(Cannon* owner, float width = 20.0f);
	~Projectile();

	// Game Object class
	void Tick(const int screenWidth, const int screenHeight) override;
	void OnDraw() const override;

	// Consumable Object class
	void OnTranslate() override;
	void OnDestroySelf() override;

	// ICollision class
	bool IsOverlapped(GameObject* target) override;

	void SetActive(bool result);

private:
	// ICollision class
	bool OnLineLineDetection(const Vector& A,
							 const Vector& B,
							 const Vector& targetPointA,
							 const Vector& targetPointB) override;

	bool OutOfScene();

	bool m_isActive;

	float m_width;

	Vector m_pointA;
	Vector m_pointB;

	Vector m_velocity;

	GameObject* m_owner;
};