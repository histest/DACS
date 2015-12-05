#include "datedlg.h"

datedlg::datedlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
}
void datedlg:: on_btn_ok_clicked()
{
	QDate date=ui.calendarWidget->selectedDate();
	strDate=date.toString("yyyy-MM-dd");
	QString strtime = ui.timeEdit->time().toString(" hh:mm:ss");
	strDate +=strtime;
	accept();
}
void datedlg:: on_btn_quit_clicked()
{
	this->close();
}
datedlg::~datedlg()
{

}
