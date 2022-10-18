#pragma once

class Vector2 {
public:
	int x;
	int y;
public:
	Vector2();
	Vector2(int x, int y);

	static Vector2 Zero();

	static Vector2 Up();
	static Vector2 Left();
	static Vector2 Down();
	static Vector2 Right();

	friend bool operator==(const Vector2& left, const Vector2& right) {
		return (left.x == right.x && left.y == right.y);
	}
	friend bool operator!=(const Vector2& left, const Vector2& right) {
		return (left.x != right.x || left.y != right.y);
	}

};

enum Direction {
	DIRECTION_LEFT = -1,
	DIRECTION_RIGHT = 1
};

enum class Axis
{
	Horizontal,
	Vertical
};

const int Sign(const int i);
const float Sign(const float f);

class AABB {
private:
	int m_x1, m_y1, m_x2, m_y2;
public:
	AABB(int x1, int y1, int x2, int y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2) {}
	int X1() { return m_x1; }
	int Y1() { return m_y1; }
	int X2() { return m_x2; }
	int Y2() { return m_y2; }
	int Width() { return m_x2 - m_x1; }
	int Height() { return m_y2 - m_y1; }
};

class StraightLine {
private:
	int m_position, m_start, m_end;
public:
	StraightLine(int start, int end) : m_start(start), m_end(end) {}
	int Position() { return m_position; }
	int Start() { return m_start; }
	int End() { return m_end; }
	int Length() { return m_end - m_start; }
};

class HorizontalLine : public StraightLine {
private:
	int m_y;
public:
	HorizontalLine(int y, int start, int end) : StraightLine(start, end), m_y(y) {}
	int Y() { return m_y; }
};

class VerticalLine : public StraightLine {
private:
	int m_x;
public:
	VerticalLine() : StraightLine(0,0), m_x(0) {}
	VerticalLine(int x, int start, int end) : StraightLine(start, end), m_x(x) {}
	int X() { return m_x; }

};

class CollisionBox {
private:
	//
public:
	VerticalLine m_left;
	VerticalLine m_right;
	HorizontalLine m_top;
	HorizontalLine m_bottom;
private:
	//
public:
	bool IsTouchingLeft();
	bool IsTouchingRight();
	bool IsTouchingTop();
	bool IsTouchingBottom();
	bool IsOverlapingLeft();
	bool IsOverlapingRight();
};

class LineCollider {
private:
	// An offset position from
	int m_x, m_y;
	int m_length;
	Axis m_axis;
public:
	//
private:
	//
public:
	LineCollider(int x, int y, int length, Axis axis) : m_x(x), m_y(y), m_length(length), m_axis(axis) {}
	const bool OverlapsSolid();
	
};