#pragma once
#include <iostream>
#include <vector>
#include <QtGui/QStandardItem>
#include <QtGui/QColor>
#include "vertex.h"

class C_Polygon
{
	friend class C_Vertex;
	private:
		QStandardItem* m_Root;
		std::vector<C_Vertex> m_Verts;
		QColor m_BaseColor;

		static std::pair<float, float> M_Middlepoint(const std::pair<float, float>& a, const std::pair<float, float>& b);
		static std::pair<float, float> M_Splitpoint(const std::pair<float,float>& a, const std::pair<float, float>& b, float t);
		static std::pair<float,float> M_CalculateSplinePoint(const std::pair<float, float>& a, const std::pair<float, float>& b, const std::pair<float, float>& c, float t);

	public:
		typedef std::vector<C_Vertex>::iterator iterator;
		typedef std::vector<C_Vertex>::const_iterator const_iterator;
		iterator begin() { return m_Verts.begin(); }
		iterator end() { return m_Verts.end(); }
		const_iterator begin() const { return m_Verts.begin(); }
		const_iterator end() const { return m_Verts.end(); }
		C_Polygon(QStandardItem* root);
		C_Polygon(QStandardItem* root, const QColor& bc);

		void M_Add(float x, float y, int pos);
		QStandardItem* M_Root() const;
		C_Vertex& M_Vertex(unsigned i);
		const C_Vertex& M_Vertex(unsigned i) const;
		void M_Delete(unsigned pos);
		C_Vertex& M_Last();
		std::pair<QStandardItem*, QStandardItem*> M_GetItems(const C_Vertex& v) const;
		void M_Reverse();
		int M_Size() const { return m_Verts.size(); }
		void M_Replace(int index, const std::vector<C_Vertex>& vec);

		static std::vector<std::pair<float, float> > M_CalculateSplinePoints(const std::pair<float, float>& a, const std::pair<float, float>& b, const std::pair<float, float>& c, int iterations);
};