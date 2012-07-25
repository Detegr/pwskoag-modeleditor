#include "vertex.h"
#include "polygon.h"

C_Vertex::C_Vertex(C_Polygon* p) : m_Parent(p), x(0), y(0), m_Color(QColor::fromRgbF(0.0f, 0.0f, 0.0f, 1.0f)), m_Selected(false), m_Hovering(false) {}
C_Vertex::C_Vertex(float newx, float newy, const QColor& c, C_Polygon* p) :
	m_Parent(p), x(newx), y(newy), m_Color(c), m_Selected(false), m_Hovering(false) {}

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
