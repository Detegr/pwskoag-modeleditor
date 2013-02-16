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

/*
std::pair<float, float> C_Polygon::M_Middlepoint(const std::pair<float, float>& a, const std::pair<float, float>& b)
{
	return std::make_pair(0.5f * (a.first + b.first), 0.5f * (a.second + b.second));
}
 
std::pair<float, float> C_Polygon::M_Splitpoint(const std::pair<float,float>& a, const std::pair<float, float>& b, float t)
{
	return std::make_pair(((1.0f-t) * a.first) + (t * b.first), ((1.0f-t) * a.second) + (t * b.second));
}
 
std::pair<float,float> C_Polygon::M_CalculateSplinePoint(const std::pair<float, float>& a, const std::pair<float, float>& b, const std::pair<float, float>& c, float t)
{
	return M_Splitpoint(M_Splitpoint(a, b, t), M_Splitpoint(b, c, t), t);
}
std::vector<std::pair<float, float> > C_Polygon::M_CalculateSplinePoints(const std::pair<float, float>& a, const std::pair<float, float>& b, const std::pair<float, float>& c, int iterations)
{
	std::vector<std::pair<float, float> > out;
	out.push_back(M_Middlepoint(a,b));
	std::pair<float, float> d = M_Middlepoint(c,b);
    for (int i=1; i<iterations-1; i++)
    {
            out.push_back(M_CalculateSplinePoint(a, b, d, (float) i / (float)(iterations - 1)));
    }
	out.push_back(b);

	return out;
}
*/
std::vector<std::pair<float, float> > C_Polygon::M_CalculateSplinePoints(const std::pair<float, float>& a, const std::pair<float, float>& b, const std::pair<float, float>& c, int iterations)
{
	std::vector<std::pair<float, float> > ret;

	for(int i=0; i<=100; i+=100/iterations)
	{
		double t = i*0.01;
		float x=(1-t)*(1-t)*a.first + 2 * (1-t) * t * b.first + (t*t) * c.first;
		float y=(1-t)*(1-t)*a.second + 2 * (1-t) * t * b.second + (t*t) * c.second;
		ret.push_back(std::make_pair(x,y));
	}
	return ret;
}