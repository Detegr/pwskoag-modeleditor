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
		Mode m_Mode;
		C_Polygon m_Polygon;
		static const unsigned m_GridSize=20;
		float m_Grid[m_GridSize];
		std::pair<float, float> m_LastClick;
		std::pair<float, float> m_LastMousePos;
		float m_DragPoints[4];
		unsigned m_SelectedPoints;
		C_Vertex* m_Single;
		bool m_Drag;
	signals:
		void S_MousePressed(float,float);
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int w, int h);
		void mousePressEvent(QMouseEvent* e);
		void mouseReleaseEvent(QMouseEvent* e);
		void mouseMoveEvent(QMouseEvent* e);
	public:
		C_GLEditor(QWidget* parent=NULL);
		~C_GLEditor() {}
		float M_RoundToPrecision(float num, unsigned precision=2);
		void M_Center();
		void M_Dump() const;
		bool M_PointInsideBox(float px, float py, float x1, float y1, float x2, float y2);
};
