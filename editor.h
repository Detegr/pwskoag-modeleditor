#include <QtGui/QtGui>
#include "editor_gl.h"

class C_Editor : public QWidget
{
	Q_OBJECT
	private:
		QPushButton* m_Center;
		QPushButton* m_Save;
		QPushButton* m_Btn3;
		C_GLEditor* m_Editor;
	private slots:
		void S_Center();
		void S_Save();
	public:
		C_Editor();
};
