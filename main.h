#pragma once
#include <QtGui/QtGui>
#include <iostream>

class C_Main : public QMainWindow
{
	Q_OBJECT
	private:
		QWidget* m_MainWidget;
		QFileDialog* m_Open;
		QFileDialog* m_Save;
	private slots:
		void S_OpenDialog()
		{
			m_Open->setVisible(true);
		}
		void S_SaveDialog()
		{
			m_Save->setVisible(true);
		}
	public:
		C_Main(QWidget* c);
		~C_Main();
};
