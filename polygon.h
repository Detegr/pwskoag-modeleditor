#pragma once
#include <vector>
#include "vertex.h"

class C_Polygon
{
	private:
	public:
		QStandardItem* m_Root;
		std::vector<C_Vertex> m_Verts; // Temporary...
		C_Polygon(QStandardItem* root) : m_Root(root) {}
		void M_Add(const C_Vertex& vert) { m_Verts.push_back(vert); }
};
