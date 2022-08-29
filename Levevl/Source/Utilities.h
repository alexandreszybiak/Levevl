#pragma once

enum Direction {
	DIRECTION_LEFT = -1,
	DIRECTION_RIGHT = 1
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
	VerticalLine(int x, int start, int end) : StraightLine(start, end), m_x(x) {}
	int X() { return m_x; }

};