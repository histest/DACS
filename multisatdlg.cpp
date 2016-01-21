#include "multisatdlg.h"
#include "connectsql.h"
extern ConnectSql sql;
MultiSatDlg::MultiSatDlg(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::MultiSatDlg)
{
	ui->setupUi(this);
	initUI();
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 0.5px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}");
}

void  MultiSatDlg::initUI()
{
	iCount=0;
	verticalSpacer = new QSpacerItem(20, 250, QSizePolicy::Minimum, QSizePolicy::Expanding);
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	bool Isexist=false;
	while(query.next())
	{
		for (int i=0;i<satlist.count();i++)
		{
			if (query.value(1).toString()==satlist.at(i))
				Isexist=true;
		}
		if(!Isexist)
		{
			satlist.append(query.value(1).toString());
		}
		Isexist=false;
	}
	on_addSatButton_clicked();
}
void  MultiSatDlg::on_okButton_clicked()
{
	QString strsql;
	QString startDate;
	QString endDate;
	for (int i=0;i<startlineedits.length();i++)
	{
		strsql = "(SATELLITENO = '"+satcomboxs.at(i)->currentText()+"'";
		startDate = startlineedits.at(i)->text();
		endDate = endlineedits.at(i)->text();
		if (sql.dboracle==true)
		{
			strsql += " and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"' )";
		}
		else
		{
			strsql += " and DATETIME between #"+startDate+"# and #"+endDate+"# )";
		}
		sqllist.append(strsql);
	}
	accept();
}
void  MultiSatDlg::on_cancelButton_clicked()
{
	this->close();
}
void  MultiSatDlg::on_addButton_clicked()
{

}
void  MultiSatDlg::on_delButton_clicked()
{

}
void  MultiSatDlg::on_addSatButton_clicked()
{
	ui->verticalLayout->removeItem(verticalSpacer);
	QHBoxLayout *hlayout = new QHBoxLayout;
	QComboBox*satcombox= new QComboBox;
	QLabel*label = new QLabel;

	QDateTime now = QDateTime::currentDateTime();
	QString endtime = now.toString("yyyy-MM-dd hh:mm:ss");  

	QDateTime start = now.addYears(-1);
	QString starttime = start.toString("yyyy-MM-dd hh:mm:ss");  

	label->setText(QString::fromLocal8Bit("名称"));
	label->setFixedWidth(48);
	satcombox->addItems(satlist);
	satcombox->setFixedWidth(200);
	satcomboxs.append(satcombox);
	hlayout->addWidget(label);
	hlayout->addWidget(satcombox);

	QHBoxLayout *hlayout2 = new QHBoxLayout;

	QLabel*startTimelabel = new QLabel;
	startTimelabel->setText(QString::fromLocal8Bit("起始时间"));

	QLineEdit*startTimeEdit = new QLineEdit;
	startTimeEdit->setFixedHeight(20);
	startTimeEdit->setFixedWidth(200);
	startTimeEdit->setText(starttime);
	startlineedits.append(startTimeEdit);

	QPushButton*startButton = new QPushButton;
	startButton->setIcon(QPixmap(":/skin/icons/light/appbar.calendar.png"));
	startButton->setIconSize(QSize(25,25));
	startButton->setFixedHeight(20);
	startButton->setFixedWidth(21);
	startButtons.append(startButton);


	hlayout2->addWidget(startTimelabel);
	hlayout2->addWidget(startTimeEdit);
	hlayout2->addWidget(startButton);

	QHBoxLayout *hlayout3 = new QHBoxLayout;
	QLabel*endTimelabel = new QLabel;
	endTimelabel->setText(QString::fromLocal8Bit("结束时间"));
	QLineEdit*endTimeEdit = new QLineEdit;
	endTimeEdit->setFixedHeight(20);
	endTimeEdit->setFixedWidth(200);
	endTimeEdit->setText(endtime);
	endlineedits.append(endTimeEdit);

	QPushButton*endButton = new QPushButton;
	endButton->setIcon(QPixmap(":/skin/icons/light/appbar.calendar.png"));
	endButton->setIconSize(QSize(25,25));
	endButton->setFixedHeight(20);
	endButton->setFixedWidth(21);
	endButtons.append(endButton);

	hlayout3->addWidget(endTimelabel);
	hlayout3->addWidget(endTimeEdit);
	hlayout3->addWidget(endButton);


	ui->verticalLayout->addLayout(hlayout);
	ui->verticalLayout->addLayout(hlayout2);
	ui->verticalLayout->addLayout(hlayout3);
	ui->verticalLayout->addItem(verticalSpacer);
	iCount++;
	if (iCount>1)
	{

		this->setFixedHeight(this->height()+100);
		QRect rect(10,40,250,100*iCount);
		//ui->verticalLayout->setGeometry(rect);
	}
	for (int i =0;i<startButtons.length();i++)
	{
		connect(startButtons[i], SIGNAL(clicked()), this, SLOT(setStartTime()));
		
	}
	for (int i =0;i<endButtons.length();i++)
	{
		connect(endButtons[i], SIGNAL(clicked()), this, SLOT(setEndTime()));
	}
}
void MultiSatDlg::setStartTime()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		for (int i = 0; i < startButtons.size(); ++i) 
		{
			if (startButtons.at(i) == sender())
			{
				startlineedits.at(i)->setText(dateDlg->strDate);
			}
		}
		
	}
}
void MultiSatDlg::setEndTime()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		for (int i = 0; i < endButtons.size(); ++i) 
		{
			if (endButtons.at(i) == sender())
			{
				endlineedits.at(i)->setText(dateDlg->strDate);
			}
		}

	}
}
MultiSatDlg::~MultiSatDlg()
{

}
