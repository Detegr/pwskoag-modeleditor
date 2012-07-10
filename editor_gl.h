#include <QtGui/QtGui>
#include <QGLWidget>
#include <vector>

class C_GLEditor : public QGLWidget
{
	Q_OBJECT
	private:
		static const unsigned m_GridSize=12;
		float m_Grid[m_GridSize];
		std::vector<std::pair<float,float> > m_Points;
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int w, int h);
		void mousePressEvent(QMouseEvent* e);
	public:
		C_GLEditor(QWidget* parent=NULL);
		~C_GLEditor() {}
		void M_Center();
		void M_Dump() const;
};
