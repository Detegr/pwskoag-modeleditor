#pragma once
#include <QtGui/QtGui>
#include <iostream>

class C_Main : public QMainWindow
{
	Q_OBJECT
	private:
		QFileDialog* m_Open;
		QFileDialog* m_Save;
		QShortcut* m_Split;
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


			QMenu* modebar=menuBar()->addMenu(tr("&Display mode"));
			QAction* poly=new QAction(tr("&Polygon"), this);
			QAction* linestrip=new QAction(tr("Line &strip"), this);
			QAction* lineloop=new QAction(tr("Line &loop"), this);

			QActionGroup* modegroup=new QActionGroup(this);

			poly->setActionGroup(modegroup);
			lineloop->setActionGroup(modegroup);
			linestrip->setActionGroup(modegroup);

			poly->setCheckable(true);
			linestrip->setCheckable(true);
			lineloop->setCheckable(true);
			lineloop->setChecked(true);

			QObject::connect(modegroup, SIGNAL(triggered(QAction*)), c, SLOT(S_ModeChanged(QAction*)));

			modebar->addAction(poly);
			modebar->addAction(linestrip);
			modebar->addAction(lineloop);

			QMenu* editbar=menuBar()->addMenu(tr("&Edit"));
			QAction* split=new QAction(tr("&Split selection"), this);
			QObject::connect(c, SIGNAL(S_SplitPossible(bool)), split, SLOT(setEnabled(bool)));
			QObject::connect(split, SIGNAL(triggered()), c, SLOT(S_Split()));
			split->setEnabled(false);
			editbar->addAction(split);

			m_Split = new QShortcut(QKeySequence(tr("Ctrl+X")), this);
			split->setShortcut(QKeySequence(tr("Ctrl+X")));

			bar->addAction(open);
			bar->addAction(save);
			bar->addAction(quit);

			setCentralWidget(c);
			show();
		}
};
