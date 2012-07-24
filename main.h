#pragma once
#include <QtGui/QtGui>
#include <iostream>

class C_Main : public QMainWindow
{
	Q_OBJECT
	private:
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
		C_Main(QWidget* c)
		{
			m_Open=new QFileDialog(this);
			m_Open->setNameFilter("*.2dmodel");
			m_Open->setAcceptMode(QFileDialog::AcceptOpen);
			m_Open->setFileMode(QFileDialog::ExistingFile);
			m_Open->setOption(QFileDialog::DontUseNativeDialog);
			QObject::connect(m_Open, SIGNAL(fileSelected(const QString&)), c, SLOT(S_OpenFile(const QString&)));
			m_Save=new QFileDialog(this);
			m_Save->setFileMode(QFileDialog::AnyFile);
			m_Save->setAcceptMode(QFileDialog::AcceptSave);
			m_Save->setDefaultSuffix("2dmodel");
			m_Save->setNameFilter("*.2dmodel");
			m_Save->setConfirmOverwrite(true);
			m_Save->setOption(QFileDialog::DontUseNativeDialog);
			QObject::connect(m_Save, SIGNAL(fileSelected(const QString&)), c, SLOT(S_SaveFile(const QString&)));
			QMenu* bar=menuBar()->addMenu(tr("&File"));
			QAction* open=new QAction(tr("&Open"), this);
			open->setShortcuts(QKeySequence::Open);
			QObject::connect(open, SIGNAL(triggered()), this, SLOT(S_OpenDialog()));
			QAction* save=new QAction(tr("&Save"), this);
			save->setShortcuts(QKeySequence::Save);
			QObject::connect(save, SIGNAL(triggered()), this, SLOT(S_SaveDialog()));
			QAction* quit=new QAction(tr("&Quit"), this);
			QObject::connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
			quit->setShortcuts(QKeySequence::Quit);

			bar->addAction(open);
			bar->addAction(save);
			bar->addAction(quit);

			setCentralWidget(c);
			show();
		}
};
