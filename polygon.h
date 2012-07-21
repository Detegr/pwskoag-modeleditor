#pragma once
#include <vector>
#include "vertex.h"

class C_Polygon
{
	private:
		QStandardItem* m_Root;
		std::vector<C_Vertex> m_Verts;
		QColor m_BaseColor;
	public:
		C_Polygon(QStandardItem* root);
		C_Polygon(QStandardItem* root, const QColor& bc);

		void M_Add(const C_Vertex& vert);
		QStandardItem* M_Root() const;
		const C_Vertex& M_Vertex(unsigned i) const;
};
