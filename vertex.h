#pragma once
#include <vector>

class C_Vertex
{
	private:
		float x,y;
		float r,g,b;
	public:
		C_Vertex();
		C_Vertex(float newx, float newy);
		C_Vertex(float newx, float newy, float newr, float newg, float newb);
		void M_SetColor(float r, float g, float b);
		void M_SetPos(float x, float y);
		std::pair<float, float> M_Pos() const;
};
