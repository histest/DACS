#include "sourcedatamanagement.h"
#include "connectsql.h"
extern ConnectSql sql;
SourceDataManagement::SourceDataManagement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 0px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");
	connect(ui.tableView, SIGNAL(clicked ( const QModelIndex &)), this,SLOT(showDictionary(const QModelIndex &)));
}

void SourceDataManagement::initUI()
{
	on_refreshButton_clicked();
	ui.tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
}
void SourceDataManagement::on_refreshButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	model->setQuery("select * from SOURCEDATA order by ID",*sql.db);
	model->setHeaderData(0,Qt::Horizontal,tr("ID"));
	model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("时间"));
	model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("卫星编号"));
	model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("经度"));
	model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("纬度"));
	model->setHeaderData(5,Qt::Horizontal,tr("Sun_x"));
	model->setHeaderData(6,Qt::Horizontal,tr("Sun_y"));
	model->setHeaderData(7,Qt::Horizontal,tr("Sun_z"));
	model->setHeaderData(8,Qt::Horizontal,tr("Sun_s"));
	if (model)
	{
		QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
		proxy->setSourceModel(model);
		ui.tableView->setModel(proxy);
	}
	ui.tableView->setSortingEnabled(true);
}
void SourceDataManagement::on_clearButton_clicked()
{
	QSqlQueryModel *model=new QSqlQueryModel();
	ui.tableView->setModel(model);
}
void SourceDataManagement::on_advancedButton_clicked()
{
	search= new Advancedsearch;
	connect(search,SIGNAL(getsql(QString)), this, SLOT(advancedpreview(QString)));
	search->setWindowModality(Qt::WindowModal);
	search->setFlag(2);
}
void SourceDataManagement::advancedpreview(QString strsql)
{
	QSqlQueryModel *model=new QSqlQueryModel();
	model->setQuery(strsql,*sql.db);
	if (model)
	{
		QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
		proxy->setSourceModel(model);
		ui.tableView->setModel(proxy);
	}
	ui.tableView->setSortingEnabled(true);
	ui.tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
}
void SourceDataManagement::showDictionary(const QModelIndex &index)
{
	QString strvalue = ui.tableView->model()->data(index).toString();
	QString strname = ui.tableView->model()->headerData(index.column(),Qt::Horizontal,0).toString();
	QString strsql =QString("select * from DICTIONARY where NAME = '"+strname+"' and %1 =  '"+strvalue+"' order by ID").arg(QString::fromLocal8Bit("数值"));
	QSqlQuery querypara(*sql.db);
	querypara.exec(strsql);
	while(querypara.next())
	{
		QToolTip::showText(QCursor::pos(), querypara.value(3).toString()); 
	}

}
SourceDataManagement::~SourceDataManagement()
{

}
