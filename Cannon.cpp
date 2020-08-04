#include "Cannon.h"
#include "graphics.h"

#include "C_Application.h"

const float k_DELAY_TIME = 0.05f;

// 1. Place the cannon (triangle) to the middle of the bottom part of the screen (window).
Cannon::Cannon(int screenWidth, int screenHeight) : m_pointA((screenWidth /2), (screenHeight -32), 0)
												  , m_pointB(((screenWidth /2) -10), (screenHeight -2), 0) 
												  , m_pointC((screenWidth /2) +10, (screenHeight -2), 0)
												  , m_triangleCenterPoint(0.0f, 0.0f, 0.0f)
												  , m_angleInDegree(0.0f)
												  , m_DeltaTime(0.0f)
												  , m_projectileIndex(0)
												  , m_lastTranslationKeyPressed(0)
												  , m_lastRotationKeyPressed(0)
												  , m_rotationMatrix(Matrix::IdentityMatrix())
{
	// Base class member variables.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_direction = Vector(0, 0, 0);
	m_localToWorld = Matrix::IdentityMatrix();

	// Initialize the Cannon information.
	UpdateTriangleCenterPoint();
	UpdateDirection();
	Transform(Matrix::TranslationMatrix(m_triangleCenterPoint));

	// Initialize the Projectile list.
	InitProjectiles();
}

Cannon::~Cannon()
{
	for (int i = 0; i < m_projectileListSize; ++i)
	{
		delete m_projectileList[i];
	}
}

void Cannon::Tick(const int screenWidth, const int screenHeight)
{
	UpdateTriangleCenterPoint();
	UpdateDirection();

	OnDraw();

	for (int projectileIndex = 0;
		 projectileIndex < (int)m_projectileListSize;
		 ++projectileIndex)
	{
		m_projectileList[projectileIndex]->Tick(m_screenWidth, m_screenHeight);
	}
}

void Cannon::OnDraw() const
{
	// Point A -> Point B
	DrawLine( (int)roundf(m_pointA.x) 
			, (int)roundf(m_pointA.y)
			, (int)roundf(m_pointB.x)
			, (int)roundf(m_pointB.y)
			, GetRGB(255, 0, 0));

	// Point A -> Point C
	DrawLine( (int)roundf(m_pointA.x)
			, (int)roundf(m_pointA.y)
			, (int)roundf(m_pointC.x)
			, (int)roundf(m_pointC.y)
			, GetRGB(0, 255, 0));


	// Point B -> Point C
	DrawLine( (int)roundf(m_pointB.x)
			, (int)roundf(m_pointB.y)
			, (int)roundf(m_pointC.x)
			, (int)roundf(m_pointC.y)
			, GetRGB(0, 0, 255));
}

// Reset back to initial stage and position.
void Cannon::OnDestroySelf()
{
	m_pointA = Vector((m_screenWidth / 2), (m_screenHeight - 32), 0);
	m_pointB = Vector(((m_screenWidth / 2) - 10), (m_screenHeight - 2), 0);
	m_pointC = Vector((m_screenWidth / 2) + 10, (m_screenHeight - 2), 0);

	UpdateTriangleCenterPoint();
	UpdateDirection();
	Transform(Matrix::TranslationMatrix(m_triangleCenterPoint));
}

void Cannon::OnTranslate(const unsigned int keyType, 
						 float speed)
{
	/*
		Note: The reason why doing the boundary check here instead of
			  using min() and max() to update each point is because
			  that will change the triangle form by not updating the point at the boundary
			  but the remaining points do.

		Prevent the Cannon moves out of boundary.
	*/
	if (m_lastTranslationKeyPressed == keyType)
	{
		if ((m_pointA.x <= 0 || m_pointA.x >= m_screenWidth)
		 || (m_pointA.y <= 0 || m_pointA.y >= m_screenHeight)
		 || (m_pointB.x <= 0 || m_pointB.x >= m_screenWidth)
		 || (m_pointB.y <= 0 || m_pointB.y >= m_screenHeight)
		 || (m_pointC.x <= 0 || m_pointC.x >= m_screenWidth)
		 || (m_pointC.y <= 0 || m_pointC.y >= m_screenHeight))
		{
			return;
		}
	}
	
	// Update the last key pressed data, for boundary check.
	m_lastTranslationKeyPressed = keyType;

	Matrix directionMatrix = Matrix::TranslationMatrix(m_direction);
	Transform(directionMatrix);

	const float OFFSET = (keyType == C_Application::s_KeyUp) ? (speed) : (-speed);

	if (roundf(m_direction.y) < 0)
	{
		m_pointA.y -= OFFSET;
		m_pointB.y -= OFFSET;
		m_pointC.y -= OFFSET;
	}

	else if (roundf(m_direction.y) > 0)
	{
		m_pointA.y += OFFSET;
		m_pointB.y += OFFSET;
		m_pointC.y += OFFSET;
	}

	if (roundf(m_direction.x) < 0)
	{
		m_pointA.x -= OFFSET;
		m_pointB.x -= OFFSET;
		m_pointC.x -= OFFSET;
	}

	else if (roundf(m_direction.x) > 0)
	{
		m_pointA.x += OFFSET;
		m_pointB.x += OFFSET;
		m_pointC.x += OFFSET;
	}
}

/*
	2. Make sure the cannon smoothly turns with the help of the cursor keys
	(to the left, to the right) in a range of -90 to 90 degrees from
	the basic direction (the basic direction is upwards).
*/
// Angle in degrees.
void Cannon::OnRotate(const unsigned int keyType, float angleOffset)
{
	if (m_lastRotationKeyPressed == keyType)
	{
		if ((m_pointA.x <= 0 || m_pointA.x >= m_screenWidth)
			|| (m_pointA.y <= 0 || m_pointA.y >= m_screenHeight)
			|| (m_pointB.x <= 0 || m_pointB.x >= m_screenWidth)
			|| (m_pointB.y <= 0 || m_pointB.y >= m_screenHeight)
			|| (m_pointC.x <= 0 || m_pointC.x >= m_screenWidth)
			|| (m_pointC.y <= 0 || m_pointC.y >= m_screenHeight))
		{
			return;
		}
	}

	// Update the last key pressed data, for boundary check.
	m_lastRotationKeyPressed = keyType;

	UpdateAngleInDegree(keyType, angleOffset);
	UpdateRotationMatrix();

	float angleInRadian = (m_angleInDegree * C_Application::k_PI) / 180.0f;

	Vector A;
	Vector B;
	Vector C;

	// Point A
	A.x = (	(m_pointA.x - m_triangleCenterPoint.x) * cos(angleInRadian) -
			(m_pointA.y - m_triangleCenterPoint.y) * sin(angleInRadian) +
			m_triangleCenterPoint.x);

	A.y = (	(m_pointA.x - m_triangleCenterPoint.x) * sin(angleInRadian) +
			(m_pointA.y - m_triangleCenterPoint.y) * cos(angleInRadian) +
			m_triangleCenterPoint.y);

	// Point B
	B.x = (	(m_pointB.x - m_triangleCenterPoint.x) * cos(angleInRadian) -
			(m_pointB.y - m_triangleCenterPoint.y) * sin(angleInRadian) +
			 m_triangleCenterPoint.x);

	B.y = (	(m_pointB.x - m_triangleCenterPoint.x) * sin(angleInRadian) +
			(m_pointB.y - m_triangleCenterPoint.y) * cos(angleInRadian) +
			 m_triangleCenterPoint.y);

	// Point C
	C.x = (	(m_pointC.x - m_triangleCenterPoint.x) * cos(angleInRadian) -
			(m_pointC.y - m_triangleCenterPoint.y) * sin(angleInRadian) +
			 m_triangleCenterPoint.x);

	C.y = (	(m_pointC.x - m_triangleCenterPoint.x) * sin(angleInRadian) +
			(m_pointC.y - m_triangleCenterPoint.y) * cos(angleInRadian) +
			 m_triangleCenterPoint.y);

	m_pointA.x = A.x;
	m_pointA.y = A.y;

	m_pointB.x = B.x;
	m_pointB.y = B.y;

	m_pointC.x = C.x;
	m_pointC.y = C.y;
}

bool Cannon::IsOverlapped(GameObject* target)
{
	Box* targetBox = (Box*)target;

	Vector targetTopLeftPoint = targetBox->GetTopLeftPoint();
	Vector targetBottomRightPoint = targetBox->GetBottomRightPoint();

	bool A = m_pointA.x >= targetTopLeftPoint.x &&
			 m_pointA.x <= targetBottomRightPoint.x &&
			 m_pointA.y >= targetTopLeftPoint.y &&
			 m_pointA.y <= targetBottomRightPoint.y;

	bool B = m_pointB.x >= targetTopLeftPoint.x &&
			 m_pointB.x <= targetBottomRightPoint.x &&
			 m_pointB.y >= targetTopLeftPoint.y &&
			 m_pointB.y <= targetBottomRightPoint.y;

	bool C = m_pointC.x >= targetTopLeftPoint.x &&
			 m_pointC.x <= targetBottomRightPoint.x &&
			 m_pointC.y >= targetTopLeftPoint.y &&
			 m_pointC.y <= targetBottomRightPoint.y;
	
	return A || B || C;
}

/*
	3. Pushing the SPACE key makes the cannon shoot a projectile –
	   a small object consisting of lines.
	   The projectile flies in the direction the cannon is turned to.

	4. It has to be possible to shoot several projectiles if the player holds the SPACE.
*/
void Cannon::OnAttack(bool isAttacking)
{
	m_DeltaTime += 0.01f;

	if (m_DeltaTime >= k_DELAY_TIME)
	{
		// Get the first available projectile object, and update its spawn position.
		m_projectileIndex = (m_projectileIndex % 
							 m_projectileListSize);

		Projectile* projectile = m_projectileList[m_projectileIndex];

		if (isAttacking)
		{
			projectile->SetActive(true);

			// Set the initial position as the owner's current spawn position.
			projectile->Transform(Matrix::TranslationMatrix(m_pointA));
			projectile->Transform(m_rotationMatrix);

			++m_projectileIndex;
		}

		m_DeltaTime = 0.0f;
	}
}

void Cannon::UpdateDirection()
{
	m_direction = m_pointA - m_triangleCenterPoint;

	// Normalize the direction.
	Vector::normalize(m_direction);
}

void Cannon::UpdateAngleInDegree(const unsigned int keyType, float angleOffset)
{
	if (keyType == C_Application::s_KeyLeft)
	{
		if (m_angleInDegree > 0)
		{
			m_angleInDegree = 0.0f;
		}

		// -90.0f
		m_angleInDegree -= angleOffset;
	}

	else
	{
		if (m_angleInDegree < 0)
		{
			m_angleInDegree = 0.0f;
		}

		// 90.0f
		m_angleInDegree += 0.1f;
	}
}

void Cannon::UpdateRotationMatrix()
{
	m_rotationMatrix = Matrix::IdentityMatrix();

	switch ((int)m_angleInDegree)
	{
		// Turn Left
	case (-90):
	{
		m_rotationMatrix = { 0.0f,	1.0f,	0.0f,
						    -1.0f, 	0.0f,	0.0f,
						     0.0f,	0.0f,	1.0f };
	}
	break;

	// Turn right
	case (90):
	{
		m_rotationMatrix = { 0.0f,	-1.0f,	0.0f,
						     1.0f, 	0.0f,	0.0f,
						     0.0f,	0.0f,	1.0f };
	}
	break;

	default:
	{
		m_rotationMatrix = Matrix::IdentityMatrix();
	}
	break;
	}

	Transform(m_rotationMatrix);
}

void Cannon::UpdateTriangleCenterPoint()
{
	// Center of Gravity = (V1 + V2 + V3) /3;

	// Make a copy of the current points.
	Vector A(m_pointA.x / 3, 
		     m_pointA.y / 3,
		     m_pointA.w / 3);

	Vector B(m_pointB.x / 3,
			 m_pointB.y / 3,
			 m_pointB.w / 3);

	Vector C(m_pointC.x / 3,
			 m_pointC.y / 3,
			 m_pointC.w / 3);

	m_triangleCenterPoint = A + B + C;
}

void Cannon::InitProjectiles()
{
	// For this game, we know there is a limit number of projectiles will be drawn on the screen.
	// So the size of the projectile list is fixed.
	m_projectileListSize =	(sizeof m_projectileList) /
							(sizeof *m_projectileList);

	for (int projectileIndex = 0; 
		 projectileIndex < (int)m_projectileListSize;
		 ++projectileIndex)
	{
		m_projectileList[projectileIndex] = new Projectile(this);
	}
}
