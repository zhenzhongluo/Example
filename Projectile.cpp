#include "graphics.h"

#include "box.h"
#include "Cannon.h"
#include "Projectile.h"


Projectile::Projectile()
{
}

Projectile::Projectile(Cannon* owner, float width) : m_pointA(0, 0, 1.0f)
												   , m_pointB(1, width, 1.0f)
												   , m_velocity(Vector(0, -1.0f, 0))
												   , m_owner(owner)
												   , m_isActive(false)
												   , m_width(width)
{
	// Base class member variables.
	m_direction = m_owner->GetDirection();
	m_localToWorld = Matrix::IdentityMatrix();
}

Projectile::~Projectile()
{
}

void Projectile::Tick(const int screenWidth, const int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	if (!OutOfScene() && m_isActive)
	{
		// shoot it at the owner's (Cannon) direction.
		Matrix directionMatrix = Matrix::TranslationMatrix(m_direction);
		Transform(directionMatrix);

		OnDraw();
		OnTranslate();
	}

	// It's either out of scene, collides with something, or just player is not attacking.
	else
	{
		OnDestroySelf();
	}
}

void Projectile::OnDraw() const
{
	Vector transA = m_localToWorld * m_pointA;
	transA.y -= 5;

	Vector transB;
	transB.x = transA.x + m_direction.x;
	transB.y = transA.y + m_direction.y;
	transB.w = transA.w + m_direction.w;

	DrawLine(transA.x,
			 transA.y,
			 transB.x,
			 transB.y,
			 GetRGB(255, 255, 255)); // White color


	transA.x += 5 + m_direction.x;
	transA.y += m_direction.y;
	transB.x = transA.x + m_direction.x +5;
	transB.y = transA.y + m_direction.y +5;
	transB.w = transA.w + m_direction.w;

	DrawLine(transA.x,
			 transA.y, 
			 transB.x,
			 transB.y,
			 GetRGB(255, 0, 0)); // Red color
}

void Projectile::OnTranslate()
{
	// Move the current projectile with velocity.
	Matrix trans = Matrix::TranslationMatrix(m_velocity);
	Transform(trans);
}

bool Projectile::IsOverlapped(GameObject* target)
{
	Box* targetBox = (Box*)target;

	Vector topLeftPoint = targetBox->GetTopLeftPoint();
	Vector topRightPoint = targetBox->GetTopRightPoint();
	Vector bottomLeftPoint = targetBox->GetBottomLeftPoint();
	Vector bottomRightPoint = targetBox->GetBottomRightPoint();

	Vector transA = m_localToWorld * m_pointA;
	Vector transB = m_localToWorld * m_pointB;

	// Check each line of the box with the current projectile collision detection.
	bool left   = OnLineLineDetection(transA, transB, topLeftPoint, bottomLeftPoint);
	bool right  = OnLineLineDetection(transA, transB, topRightPoint, bottomRightPoint);
	bool top    = OnLineLineDetection(transA, transB, topLeftPoint, topRightPoint);
	bool bottom = OnLineLineDetection(transA, transB, bottomLeftPoint, bottomRightPoint);

	if (left || right || top || bottom)
	{
		return true;
	}
	return false;
}

bool Projectile::OnLineLineDetection(const Vector& A, 
									 const Vector& B, 
									 const Vector& targetPointA, 
									 const Vector& targetPointB)
{
	float x1 = A.x;
	float y1 = A.y;

	float x2 = B.x;
	float y2 = B.y;

	float x3 = targetPointA.x;
	float y3 = targetPointA.y;

	float x4 = targetPointB.x;
	float y4 = targetPointB.y;

	// calculate the direction of the lines
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) /
			   ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) /
			   ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	if (uA >= 0 && uA <= 1 &&
		uB >= 0 && uB <= 1)
	{
		return true;
	}

	return false;
}

void Projectile::SetActive(bool result)
{
	m_isActive = result;
}

// Reset back to initial stage.
void Projectile::OnDestroySelf()
{
	m_isActive = false;

	m_direction = m_owner->GetDirection();
	m_localToWorld = Matrix::IdentityMatrix();
}

// Detect if the projectile is out of scene.
bool Projectile::OutOfScene()
{
	Vector center = m_localToWorld * Vector(0.f, 0.f, 1.f);

	float halfWidth = m_width / 2.f;

	if ((center.x < halfWidth || center.x >(m_screenWidth - halfWidth)) ||
		(center.y < halfWidth || center.y >(m_screenHeight - halfWidth)))
	{
		return true;
	}

	return false;
}
