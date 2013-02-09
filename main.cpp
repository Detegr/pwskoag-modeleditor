#include <QtGui/QApplication>

#include <cstdio>
#include <cstdlib>
#include "editor.h"
#include "main.h"

C_Main::C_Main(QWidget* c) : m_MainWidget(c)
{
	QMenu* bar=menuBar()->addMenu(tr("&File"));
	QAction* open=new QAction(tr("&Open"), this);
	open->setShortcuts(QKeySequence::Open);
	QObject::connect(open, SIGNAL(triggered()), c, SLOT(S_OpenDialog()));
	QAction* save=new QAction(tr("&Save"), this);
	save->setShortcuts(QKeySequence::Save);
	QObject::connect(save, SIGNAL(triggered()), c, SLOT(S_SaveOrSaveAs()));
	QAction* saveas=new QAction(tr("Save &as"), this);
	saveas->setShortcuts(QKeySequence::SaveAs);
	QObject::connect(saveas, SIGNAL(triggered()), c, SLOT(S_SaveDialog()));
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

	QAction* reverse=new QAction(tr("&Reverse active polygon"), this);
	QObject::connect(reverse, SIGNAL(triggered()), c, SLOT(S_ReverseActivePolygon()));

	editbar->addAction(split);
	editbar->addAction(reverse);

	split->setShortcut(QKeySequence(tr("Ctrl+X")));
	reverse->setShortcut(QKeySequence(tr("Ctrl+R")));

	bar->addAction(open);
	bar->addAction(save);
	bar->addAction(saveas);
	bar->addAction(quit);

	setCentralWidget(c);
	show();
}

C_Main::~C_Main()
{
	delete m_MainWidget;
}

void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
	case QtDebugMsg:
	 fprintf(stderr, "Debug: %s\n", msg);
	 break;
	case QtWarningMsg:
	 fprintf(stderr, "Warning: %s\n", msg);
	 break;
	case QtCriticalMsg:
	 fprintf(stderr, "Critical: %s\n", msg);
	 break;
	case QtFatalMsg:
	 fprintf(stderr, "Fatal: %s\n", msg);
	 abort();
	}
}

int main(int argc, char **argv)
{
	qInstallMsgHandler(myMessageOutput);
	QApplication app(argc, argv);
	C_Main* w;
	if(argc>1)
	{
		w = new C_Main(new C_Editor(QString(argv[1])));
	}
	else w = new C_Main(new C_Editor);
	int ret=app.exec();
	return ret;
}
