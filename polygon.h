#pragma once
#include <vector>
#include "vertex.h"

class C_Polygon
{
	private:
	public:
		std::vector<C_Vertex> m_Verts; // Temporary...
		C_Polygon() {}
		void M_Add(const C_Vertex& vert) { m_Verts.push_back(vert); }
};
