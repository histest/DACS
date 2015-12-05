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
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QApplication::addLibraryPath("./plugins");
	ConnectDB connectdatase;
	//QSplashScreen *splash = new QSplashScreen;  
	//splash->setPixmap(QPixmap(":/images/skin/images/bear.jpg"));  
	//splash->show();  
	//Qt::Alignment bottteomRight = Qt::AlignRight | Qt::AlignBottom;  
	//splash->showMessage(QString::fromLocal8Bit("³ÌÐòÆô¶¯ÖÐ..."),  
	//	bottteomRight,  
	//	QColor (153,217,234));  
	//SleeperThread::msleep(3000);
 //
	//splash->finish(&connectdatase);  
	//delete splash;  

	 int currentExitCode = 0;
	if(connectdatase.exec()==QDialog::Accepted)//
	{		
		do{
			MainWindow *main = MainWindow::getInstance();
			main->initData(connectdatase.IsAdmin,connectdatase.IsAuthorized);
			main->setAttribute(Qt::WA_DeleteOnClose);			
			main->show();
			setSkinForApp(QString(":/qss/skin/qss/GB.qss"));
			currentExitCode = app.exec();

		}while( currentExitCode == EXIT_CODE_REBOOT );

	}
	
    return 0;
}
