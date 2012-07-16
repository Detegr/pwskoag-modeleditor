#pragma once
#include <vector>
#include <QtGui/QColor>

class C_Vertex
{
	private:
		float x,y;
		QColor m_Color;
		bool m_Selected;
	public:
		C_Vertex();
		C_Vertex(float newx, float newy);
		C_Vertex(float newx, float newy, const QColor& c);
		void M_SetColor(const QColor& c);
		const QColor& M_Color() const { return m_Color; }
		void M_SetSelection(bool b) { m_Selected=b; }
		bool M_Selected() { return m_Selected; }
		void M_SetPos(float x, float y);
		std::pair<float, float> M_Pos() const;
};
