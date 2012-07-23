#include <QtGui/QApplication>

#include <cstdio>
#include <cstdlib>
#include "editor.h"
#include "main.h"

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
	C_Main w(new C_Editor);
	return app.exec();
}
