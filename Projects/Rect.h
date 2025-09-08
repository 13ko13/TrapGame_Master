#pragma once
#include "Vec2.h"

class Rect
{
public:
	Rect();
	~Rect();

	void Draw(unsigned int Color, bool isFill);
	void SetLT(float left, float top, float width, float height);
	void SetCenter(float x, float y, float width, float height);
	float GetWidth() const;
	float GetHeight() const;
	Vec2 GetCenter() const;
	float GetLeft() const { return m_left; }
	float GetTop() const { return m_top; }
	float GetRight() const { return m_right; }
	float GetBottom() const { return m_bottom; }
	bool IsCollision(const Rect& rect) const;
	
public:
	float m_left;	//�����X���W
	float m_top;	//�����Y���W
	float m_right;	//�E����X���W
	float m_bottom;	//�E����Y���W
};

