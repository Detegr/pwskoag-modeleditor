#include "polygon.h"
#include <sstream>
#include <algorithm>

C_Polygon::C_Polygon(QStandardItem* root) :
	m_Root(root)
{
	m_BaseColor=QColor::fromRgbF(0.5f, 0.5f, 0.5f, 1.0f);
}

C_Polygon::C_Polygon(QStandardItem* root, const QColor& bc) : m_Root(root), m_BaseColor(bc) {}

void C_Polygon::M_Add(float x, float y, int pos)
{
	if(pos == -1)
	{
		m_Verts.push_back(C_Vertex(x,y,m_BaseColor,this));
	}
	else
	{
		m_Verts.insert(m_Verts.begin()+pos, C_Vertex(x,y,m_BaseColor,this));
	}
}

std::pair<QStandardItem*, QStandardItem*> C_Polygon::M_GetItems(const C_Vertex& v) const
{
	unsigned i=0;
	for(std::vector<C_Vertex>::const_iterator it=m_Verts.begin(); it!=m_Verts.end(); ++it, ++i)
	{
		if(&(*it) == &v)
		{
			QStandardItem* l=m_Root->child(i, 0);
			QStandardItem* r=m_Root->child(i, 1);
			return std::make_pair(l,r);
		}
	}
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

void C_Polygon::M_Delete(unsigned pos)
{
	m_Verts.erase(m_Verts.begin()+pos);
}

void C_Polygon::M_Reverse()
{
	std::reverse(m_Verts.begin(), m_Verts.end());
}

void C_Polygon::M_Replace(int index, const std::vector<C_Vertex>& vec)
{
	m_Verts.erase(m_Verts.begin()+index);
	m_Verts.insert(m_Verts.begin()+index, vec.begin(), vec.end());
}
