#include "QFramer/futil.h"
#include "mainwindow/mainwindow.h"
#include <QApplication>
#include "connectdb.h"
#include <qtextcodec.h>
#include <QTextEdit>  
#include <QSplashScreen>  
#include <QThread>
class SleeperThread : public QThread
{
public:
	static void msleep(unsigned long msecs)
	{
		QThread::msleep(msecs);
	}
};
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QApplication::setApplicationName("DCAS");
    QApplication::setWindowIcon(QIcon(":/images/skin/images/QFramer.ico"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QApplication::addLibraryPath("./plugins");
	ConnectDB connectdatase;

	 int currentExitCode = 0;

	 app.connect( &app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	if(connectdatase.exec()==QDialog::Accepted)//
	{		
		do{
			MainWindow *main = MainWindow::getInstance();
			main->connect(main,SIGNAL(CloseAll()),&app,SLOT(exit()));
			main->initData(connectdatase.IsAdmin,connectdatase.IsAuthorized);
			main->setAttribute(Qt::WA_DeleteOnClose,true);			
			main->show();
			setSkinForApp(QString(":/qss/skin/qss/GB.qss"));
			currentExitCode = app.exec();

		}while( currentExitCode == EXIT_CODE_REBOOT );

	}

    return 0;
}
