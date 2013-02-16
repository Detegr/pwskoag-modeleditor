#pragma once
#include <vector>
#include <QtGui/QColor>

class C_Polygon;
struct C_Data
{
	QString str;
	bool	isbezier;

	C_Data() : str(""), isbezier(false) {}
};

class C_Vertex
{
	private:
		C_Polygon* m_Parent;
		float x,y;
		C_Data m_Data;
		QColor m_Color;
		bool m_Selected;
		bool m_Hovering;
	public:
		C_Vertex(C_Polygon* p);
		C_Vertex(C_Polygon* p, float x, float y);
		C_Vertex(float newx, float newy, const QColor& c, C_Polygon* p);
		void M_SetColor(const QColor& c);
		const QColor& M_Color() const { return m_Color; }
		void M_SetSelection(bool b) { m_Selected=b; }
		bool M_Selected() const { return m_Selected; }
		void M_SetHovering(bool h) { m_Hovering=h; }
		bool M_Hovering() const { return m_Hovering; }
		void M_SetPos(float x, float y);
		void M_SetStrData(const QString& data);
		QString M_GetStrData() const;
		void M_SetIsBezier(bool b);
		bool M_GetIsBezier() const;
		std::pair<float, float> M_Pos() const;
};
