#include "polygon.h"
#include <sstream>

C_Polygon::C_Polygon(QStandardItem* root) :
	m_Root(root)
{
	m_BaseColor=QColor::fromRgbF(0.5f, 0.5f, 0.5f, 1.0f);
}

C_Polygon::C_Polygon(QStandardItem* root, const QColor& bc) : m_Root(root), m_BaseColor(bc) {}

void C_Polygon::M_Add(float x, float y)
{
	m_Verts.push_back(C_Vertex(x,y,m_BaseColor,this));
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

void C_Polygon::M_NewPos(const C_Vertex* v, float x, float y)
{
	int i=0;
	for(std::vector<C_Vertex>::iterator it=m_Verts.begin(); it!=m_Verts.end(); ++it, ++i)
	{
		if(&(*it) == v)
		{
			QStandardItem* l=m_Root->child(i, 0);
			QStandardItem* r=m_Root->child(i, 1);
			l->setData(x); r->setData(y);
			std::stringstream ss;
			std::string str;
			ss << x; ss >> str;
			l->setText(QString(str.c_str()));
			ss.clear();
			ss << y; ss >> str;
			r->setText(QString(str.c_str()));
		}
	}
}
