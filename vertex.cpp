#include "vertex.h"

C_Vertex::C_Vertex() : x(0), y(0), m_Color(), m_Selected(false) {}
C_Vertex::C_Vertex(float newx, float newy) : x(newx), y(newy), m_Color(), m_Selected(false) {}
C_Vertex::C_Vertex(float newx, float newy, const QColor& c) :
	x(newx), y(newy), m_Color(c), m_Selected(false) {}

void C_Vertex::M_SetColor(const QColor& c)
{
	m_Color=c;
}

void C_Vertex::M_SetPos(float x, float y)
{
	this->x=x; this->y=y;
}

std::pair<float, float> C_Vertex::M_Pos() const
{
	return std::make_pair(x,y);
}
