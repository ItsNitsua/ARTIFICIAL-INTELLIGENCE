#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__
#include "DisplayObject.h"
#include "TextureManager.h"
#include "../Template/Line.h"

class SpaceShip final : public DisplayObject
{
public:
	// constructor(s)
	SpaceShip();

	// destructor
	~SpaceShip();
	
	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
	void setWhisker(glm::vec2 start, glm::vec2 end);
	void turnRight();
	void turnLeft();
	void moveForward();
	friend class CollisionScene;
	void moveBack();
	
private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	Line m_leftWhisker;
	Line m_rightWhisker;

	// private function
	void m_Move();
	void m_changeDirection();
	void m_checkBounds();
	void m_stopOnTouch();
	friend class ArrivalScene;
};

#endif /* defined (__SPACE_SHIP__) */