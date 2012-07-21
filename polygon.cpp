#include "polygon.h"

C_Polygon::C_Polygon(QStandardItem* root) :
	m_Root(root)
{
	m_BaseColor=QColor(fromRgbF(0.5f, 0.5f, 0.5f, 1.0f));
}

C_Polygon::C_Polygon(QStandardItem* root, QBaseColor bc) : m_Root(root), m_BaseColor(bc) {}

void M_Add(const C_Vertex& vert)
{
	m_Verts.push_back(vert, m_BaseColor);
}

QStandardItem* M_Root() const
{
	return m_Root;
}

const C_Vertex& M_Vertex(unsigned i) const
{
	return m_Verts[i];
}
