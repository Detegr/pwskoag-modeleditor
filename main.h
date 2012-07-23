#pragma once
#include <QtGui/QtGui>

class C_Main : public QMainWindow
{
	Q_OBJECT
	private:

	public:
		C_Main(QWidget* c)
		{
			menuBar()->addMenu(tr("&File"));
			setCentralWidget(c);
			show();
		}
};
