#pragma once
#ifndef _LINE_H_
#define __LINE_H_

#include "glm/vec2.hpp"

struct Line // from alex's video
{
public:
	Line() {}
	void SetLine(glm::vec2 s, glm::vec2 e)
	{
		start = s;
		end = e;
	}
	glm::vec2 Start() const { return start; }
	glm::vec2 End() const { return end; }
	friend class CollisionScene;
private:
	glm::vec2 start;
	glm::vec2 end;
	//float length;
};












#endif
