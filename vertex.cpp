#include "vertex.h"

C_Vertex::C_Vertex() : x(0), y(0), r(1.0), g(1.0), b(1.0) {}
C_Vertex::C_Vertex(float newx, float newy) : x(newx), y(newy), r(1.0), g(1.0), b(1.0) {}
C_Vertex::C_Vertex(float newx, float newy, float newr, float newg, float newb) :
	x(newx), y(newy), r(newr), g(newg), b(newb) {}

void C_Vertex::M_SetColor(float r, float g, float b)
{
	this->r=r; this->g=g; this->b=b;
}
void C_Vertex::M_SetPos(float x, float y)
{
	this->x=x; this->y=y;
}

std::pair<float, float> C_Vertex::M_Pos() const
{
	return std::make_pair(x,y);
}
