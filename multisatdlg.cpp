#include "multisatdlg.h"
#include "connectsql.h"
extern ConnectSql sql;
MultiSatDlg::MultiSatDlg(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::MultiSatDlg)
{
	ui->setupUi(this);
	initUI();
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}");
}

void  MultiSatDlg::initUI()
{
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());

	}
	ui->listWidget->addItems(list);
}
void  MultiSatDlg::on_okButton_clicked()
{
	satellitecount= ui->selectlistWidget->count();
	for (int i=0;i<ui->selectlistWidget->count();i++)
	{
		satellites[i]=ui->selectlistWidget->item(i)->text();
	}
	accept();
}
void  MultiSatDlg::on_cancelButton_clicked()
{
	this->close();
}
void  MultiSatDlg::on_addButton_clicked()
{
	 QList<QListWidgetItem*> list=ui->listWidget->selectedItems();
	 for (int i=0;i<list.count();i++)
	 {
		 ui->selectlistWidget->addItem(list[i]->text());
	 }
}
void  MultiSatDlg::on_delButton_clicked()
{
	QList<QListWidgetItem*> list=ui->selectlistWidget->selectedItems();
	for (int i=0;i<list.count();i++)
	{
		ui->selectlistWidget->takeItem(list[i]->data(0).toInt());
	}//list.takeAt(i).data().toInt()
}
MultiSatDlg::~MultiSatDlg()
{

}
