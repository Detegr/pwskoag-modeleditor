#include "polygon.h"

C_Polygon::C_Polygon(QStandardItem* root) :
	m_Root(root)
{
	m_BaseColor=QColor::fromRgbF(0.5f, 0.5f, 0.5f, 1.0f);
}

C_Polygon::C_Polygon(QStandardItem* root, const QColor& bc) : m_Root(root), m_BaseColor(bc) {}

void C_Polygon::M_Add(float x, float y)
{
	m_Verts.push_back(C_Vertex(x,y,m_BaseColor));
}

QStandardItem* C_Polygon::M_Root() const
{
	return m_Root;
}

C_Vertex& C_Polygon::M_Vertex(unsigned i)
{
	return m_Verts[i];
}

const C_Vertex& C_Polygon::M_Vertex(unsigned i) const
{
	return m_Verts[i];
}

C_Vertex& C_Polygon::M_Last()
{
	return m_Verts.back();
}
