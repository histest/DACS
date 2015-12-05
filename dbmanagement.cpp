#include "dbmanagement.h"
#include "backupdb.h"
#include "recoverdb.h"
#include "deletedb.h"

DBManagement::DBManagement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	/*setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}"
	"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
	"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}");*/
	ui.backupButton->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.debug.step.into.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: white;border-style: outset;border-width: 1px;border-radius: 5px;text-align: center bottom;}");
	ui.recoverButton->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.debug.step.out.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: white;border-style: outset;border-width: 1px;border-radius: 5px;text-align: center bottom;}");
	ui.deleteButton->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.delete.png);border-color:grey;background-repeat:no-repeat;background-position: center top;color: white;border-style: outset;border-width: 1px;border-radius: 5px;text-align: center bottom;}");

}
void DBManagement::on_deleteButton_clicked()
{
	Deletedb deletedb = new Deletedb;
	deletedb.exec();
	/*QString todir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	ui.lineEdit->setText(todir);*/
}
void DBManagement::on_openButton_2_clicked()
{
	/*QString fileName = QFileDialog::getOpenFileName(this, tr("Access file"), qApp->applicationDirPath (),
		tr("Access Files (*.accdb)"));
	if (fileName.isEmpty())
		return;
	ui.lineEdit_2->setText(fileName);*/
}
void DBManagement::on_recoverButton_clicked()
{
	Recoverdb recoverdb = new Recoverdb;
	recoverdb.exec();
	//QString sourcedir =ui.lineEdit_2->text();
	//QString todir = QCoreApplication::applicationDirPath();
	//todir+="/DACS.accdb";  
	///*sourcedir+="DACS.accdb";  */
	///*bool iscopy=copyFileToPath(sourcedir,todir,true);*/
	//sourcedir.replace("\\","/");  
	//QFile::copy(sourcedir, todir);
}
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)  
{  
	//toDir.replace("\\","/");  
	//if (sourceDir == toDir){  
	//	return true;  
	//}  
	//if (!QFile::exists(sourceDir)){  
	//	return false;  
	//}  
	//QDir *createfile     = new QDir;  
	//bool exist = createfile->exists(toDir);  
	//if (exist){  
	//	if(coverFileIfExist){  
	//		createfile->remove(toDir);  
	//	}  
	//}//end if  

	//if(!QFile::copy(sourceDir, toDir))  
	//{  
	//	return false;  
	//}  
	return true;  
}  
void DBManagement::on_backupButton_clicked()
{
	Backupdb backupdb = new Backupdb;
	backupdb.exec();
	//QString todir =ui.lineEdit->text();
	//QString sourcedir = QCoreApplication::applicationDirPath();
	//sourcedir+="/DACS.accdb";  
	//todir+="DACS.accdb";  
	///*bool iscopy=copyFileToPath(sourcedir,todir,true);*/
	//todir.replace("\\","/");  
	//QFile::copy(sourcedir, todir);
}

DBManagement::~DBManagement()
{

}
