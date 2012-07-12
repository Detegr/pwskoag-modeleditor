#include <QtGui/QtGui>
#include <QGLWidget>
#include <vector>

class C_GLEditor : public QGLWidget
{
	friend class C_Editor;
	Q_OBJECT
	private:
		std::vector<std::pair<float, float> > m_Points;
		static const unsigned m_GridSize=20;
		float m_Grid[m_GridSize];
	signals:
		void S_MousePressed(float,float);
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int w, int h);
		void mousePressEvent(QMouseEvent* e);
	public:
		C_GLEditor(QWidget* parent=NULL);
		~C_GLEditor() {}
		float M_RoundToPrecision(float num, unsigned precision=2);
		void M_Center();
		void M_Dump() const;
};
