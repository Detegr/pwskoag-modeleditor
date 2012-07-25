#include <QtGui/QtGui>
#include <QGLWidget>
#include <vector>
#include "polygon.h"

class C_GLEditor : public QGLWidget
{
	friend class C_Editor;
	Q_OBJECT
	private:
		static const float m_MouseOverPrecision=0.02f;
		enum Mode
		{
			Insert, Edit
		};
		std::vector<C_Polygon*> m_Polygons;
		C_Polygon* m_ActivePoly;
		Mode m_Mode;
		static const unsigned m_GridSize=20;
		float m_Grid[m_GridSize];
		std::pair<float, float> m_LastClick;
		std::pair<float, float> m_LastMousePos;
		float m_DragPoints[4];
		bool m_Drag;

		void M_PaintGrid();
		void M_PaintPolygon(const C_Polygon& p);
		void M_PaintPoints(const C_Polygon& p);
		void M_PaintDrag();

	signals:
		void S_MousePressed(QStandardItem*, float,float);
		void S_RequestColorDialog(QList<C_Vertex*>);
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int w, int h);
		void mousePressEvent(QMouseEvent* e);
		void mouseReleaseEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
		void mouseDoubleClickEvent(QMouseEvent* e);
		bool M_MouseOverVertex(float, float, const C_Vertex& v);
	public:
		C_GLEditor(QWidget* parent, QStandardItem* root);
		~C_GLEditor();
		float M_RoundToPrecision(float num, unsigned precision=2);
		void M_Center();
		void M_Dump() const;
		bool M_PointInsideBox(float px, float py, float x1, float y1, float x2, float y2);
};
