#pragma once
#include <QtGui/QtGui>
#include <iostream>

class C_Main : public QMainWindow
{
	Q_OBJECT
	private:
		QWidget* m_MainWidget;
	public:
		C_Main(QWidget* c);
		~C_Main();
};
