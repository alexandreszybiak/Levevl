#pragma once

const int Sign(const int i);

class StraightLine {
private:
	int m_start, m_end;
public:
	StraightLine(int start, int end) : m_start(start), m_end(end) {}
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