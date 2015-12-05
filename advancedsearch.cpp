#include "advancedsearch.h"
#include "connectsql.h"
#include "multisatdlg.h"
#include <vector>  
using namespace std;
extern ConnectSql sql;
vector <QString> vtSat; 
Advancedsearch::Advancedsearch(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	namelistview = new QListView(this);
	model = new QStringListModel(this);
	namelistview->setWindowFlags(Qt::ToolTip);
	installEventFilter(namelistview);
	connect(namelistview, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeText(const QModelIndex &)));
	connect(ui.multiSatcheckBox,SIGNAL(stateChanged(int)),this,SLOT(multiSatcheck()));
	connect(ui.comboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(setCompleter(const QString &)));
	disconnect(ui.comboBox->view(),0,0,0);  
	setStyleSheet("QLineEdit{border-radius: 0px;}");
}
void Advancedsearch::initUI()
{
	vtSat.clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());
		vtSat.push_back(query.value(1).toString());

	}
	this->ui.comboBox->addItems(list);
}
void Advancedsearch::on_addButton_clicked()
{
	ui.verticalLayout->removeItem(verticalSpacer);
	QGroupBox*group = new QGroupBox;
	group->setFixedHeight(21);
	group->setFixedWidth(138);
	
	QRadioButton*andradiobutton = new QRadioButton(group);
	andradiobutton->setText(QString::fromLocal8Bit("并且"));
	andradiobutton->setGeometry(andradiobutton->x()+20,andradiobutton->y()-4,andradiobutton->width()-30,andradiobutton->height());
	andradiobutton->setChecked(true);
	andradiosbuttons.append(andradiobutton);
	QRadioButton*orradiobutton = new QRadioButton(group);
	orradiobutton->setText(QString::fromLocal8Bit("或"));
	orradiobutton->setGeometry(andradiobutton->width()+andradiobutton->x(),andradiobutton->y(),andradiobutton->width(),andradiobutton->height());
	orradiosbuttons.append(orradiobutton);
	ui.verticalLayout->addWidget(group);


	QHBoxLayout *hlayout = new QHBoxLayout;
	QComboBox*namecombox= new QComboBox;
	QComboBox*siftcombox= new QComboBox;
	namecombox->addItems(*namelist);
	siftcombox->addItems(*siftlist);
	namecomboxs.append(namecombox);
	siftcomboxs.append(siftcombox);
	QLineEdit*lineedit = new QLineEdit;
	lineedit->setFixedHeight(20);
	lineedits.append(lineedit);
	hlayout->addWidget(namecombox);
	hlayout->addWidget(siftcombox);
	hlayout->addWidget(lineedit);
	ui.verticalLayout->addLayout(hlayout);
	ui.verticalLayout->addItem(verticalSpacer);
	icount ++;
	if (icount>2)
	{
		this->setFixedHeight(this->height()+50);
		QRect rect(240,30,290,60*icount);
		ui.verticalLayout->setGeometry(rect);
	}
}
void Advancedsearch::on_okButton_clicked()
{
	int icount = lineedits.count();
	QString strsql;

	QString strSat =ui.comboBox->currentText();
	//QString startDate=ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	//QString endDate=ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  

	QString startDate=ui.startdateEdit->text();//ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString endDate=ui.enddateEdit->text();//ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  


	if (ui.multiSatcheckBox->checkState()!=Qt::Checked)
	{
		satlist.clear();
		satlist.append(ui.comboBox->currentText());
	}
	//for (int i = 0;i<satlist.count();i++)
	//{
	//	strSat = satlist.at(i);
	//	if (sql.dboracle==true)
	//	{
	//		strsql = "where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'";
	//	}
	//	else
	//	{
	//		strsql = "where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"#";//and
	//	}
	//
	//	if (lineedits.at(0)->text()!=NULL)
	//	{
	//		strsql+=" and ";
	//		strsql+=namecomboxs.at(0)->currentText()+getsift(siftcomboxs.at(0)->currentText())+lineedits.at(0)->text();
	//	}
	//	for (int i=1;i<icount;i++)
	//	{

	//		if (lineedits.at(i)->text()!=NULL)
	//		{
	//			if (andradiosbuttons.at(i-1)->isChecked())
	//			{
	//				strsql+=" and ";
	//			}
	//			if (orradiosbuttons.at(i-1)->isChecked())
	//			{
	//				strsql+=" or ";
	//			}
	//			strsql+=namecomboxs.at(i)->currentText()+" "+getsift(siftcomboxs.at(i)->currentText())+lineedits.at(i)->text();
	//		}
	//	}
	//	if (strsql!="")
	//	{
	//		strsql +="order by ID";
	//		sqllist.append(strsql);
	//	}
	//}
	strsql="where";
	int satcount = satlist.count();
	if (satcount>1)
	{
		for (int i = 0;i<satcount;i++)
		{
			strsql += " SATELLITENO = '"+satlist.at(i)+"'";
			if(i!=satlist.count()-1)
			{
				strsql +="  or  ";
			}
		}
	}
	else
	{
		strsql += " SATELLITENO = '"+satlist.at(0)+"'";
	}

	if (sql.dboracle==true)
	{
		strsql += " and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'";
	}
	else
	{
		strsql += " and DATETIME between #"+startDate+"# and #"+endDate+"#";
	}
	if (lineedits.at(0)->text()!=NULL)
	{
		strsql+=" and ";
		strsql+=namecomboxs.at(0)->currentText()+getsift(siftcomboxs.at(0)->currentText())+lineedits.at(0)->text();
	}
	for (int i=1;i<icount;i++)
	{

		if (lineedits.at(i)->text()!=NULL)
		{
			if (andradiosbuttons.at(i-1)->isChecked())
			{
				strsql+=" and ";
			}
			if (orradiosbuttons.at(i-1)->isChecked())
			{
				strsql+=" or ";
			}
			strsql+=namecomboxs.at(i)->currentText()+" "+getsift(siftcomboxs.at(i)->currentText())+lineedits.at(i)->text();
		}
	}
	strsql +="  order by ID";
	emit getsql(strsql);
	this->close();
}
QString Advancedsearch::getsift(QString strsift)
{
	if (strsift==QString::fromLocal8Bit("大于"))
	{
		strsift=">";
	}
	if (strsift==QString::fromLocal8Bit("等于"))
	{
		strsift="=";
	}
	if (strsift==QString::fromLocal8Bit("小于"))
	{
		strsift="<";
	}
	if (strsift==QString::fromLocal8Bit("包含"))
	{
		strsift="like";
	}
	return strsift;
}
void Advancedsearch::on_cancelButton_clicked()
{
	this->close();
}
void Advancedsearch::setFlag(int iFlag)
{

	QHBoxLayout *hlayout = new QHBoxLayout;
	QComboBox*namecombox= new QComboBox;
	QComboBox*siftcombox= new QComboBox;

	siftlist = new QStringList;
	namelist = new QStringList;
	siftlist->append(QString::fromLocal8Bit("大于"));
	siftlist->append(QString::fromLocal8Bit("等于"));
	siftlist->append(QString::fromLocal8Bit("小于"));
	siftlist->append(QString::fromLocal8Bit("包含"));
	namelist->append(QString::fromLocal8Bit("经度"));
	namelist->append(QString::fromLocal8Bit("纬度"));
	namelist->append(QString::fromLocal8Bit("DIGITAL_SUN_X"));
	namelist->append(QString::fromLocal8Bit("DIGITAL_SUN_Y"));
	namelist->append(QString::fromLocal8Bit("DIGITAL_SUN_Z"));
	namelist->append(QString::fromLocal8Bit("DIGITAL_SUN_S"));
	namelist->append(QString::fromLocal8Bit("累计复位次数"));
	namelist->append(QString::fromLocal8Bit("连续复位次数"));
	namelist->append(QString::fromLocal8Bit("复位原因"));	
	if (iFlag==1)
	{
		namelist->append(QString::fromLocal8Bit("单错次数"));	
	}
	if (iFlag==2)
	{
		namelist->append(QString::fromLocal8Bit("单错累计次数"));	
	}
	namelist->append(QString::fromLocal8Bit("发生错误的IO或RAM或ROM地址"));	
	namelist->append(QString::fromLocal8Bit("陷阱（Trap)类型"));	
	namelist->append(QString::fromLocal8Bit("切机标志"));	
	namelist->append(QString::fromLocal8Bit("加电标志"));	
	namelist->append(QString::fromLocal8Bit("SRAM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("PROM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("SRAM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("PROM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("ROM状态标志"));	
	namelist->append(QString::fromLocal8Bit("RAM状态标志"));	
	namelist->append(QString::fromLocal8Bit("EEPROM状态标志"));	
	namelist->append(QString::fromLocal8Bit("发生错误的地址区域"));	
	namelist->append(QString::fromLocal8Bit("EEPROM错误地址"));	
	namelist->append(QString::fromLocal8Bit("EEPROM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("EEPROM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("EEPROM错误次数"));	
	namelist->append(QString::fromLocal8Bit("MRAM双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("MRAM单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("MRAM错误计数"));	
	namelist->append(QString::fromLocal8Bit("NOR FLASH双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("NOR FLASH单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("NOR FLASH错误计数"));	
	namelist->append(QString::fromLocal8Bit("同步故障类型"));	
	namecombox->addItems(*namelist);
	siftcombox->addItems(*siftlist);

	namecomboxs.append(namecombox);
	siftcomboxs.append(siftcombox);
	QLineEdit*lineedit = new QLineEdit;
	lineedit->setFixedHeight(20);
	lineedits.append(lineedit);
	hlayout->addWidget(namecombox);
	hlayout->addWidget(siftcombox);
	hlayout->addWidget(lineedit);
	ui.verticalLayout->addLayout(hlayout);
	icount =0;
	verticalSpacer = new QSpacerItem(20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui.verticalLayout->addItem(verticalSpacer);
	this->show();
}
void Advancedsearch::multiSatcheck()
{
	satlist.clear();
	if (ui.multiSatcheckBox->checkState()==Qt::Checked)
	{
		ui.comboBox->setEnabled(false);
		MultiSatDlg multisatdlg;
		if(multisatdlg.exec()==QDialog::Accepted)
		{
			int satellitecount = multisatdlg.satellitecount;			
			for (int i=0;i<satellitecount;i++)
			{
				QString strSat=multisatdlg.satellites[i];			
				satlist.append(strSat);
				QSqlQuery query(*sql.db);
				query.exec("select*from PARAMETER where SATELLITENO = '"+strSat+"'order by ID");
				QStringList list;
				while(query.next())
				{

					QString str = str.fromLocal8Bit("-卫星编号：");
					QString strID = QString::number(i+1);
					str = strID+str;
					list.append(str+query.value(1).toString());	
					str = str.fromLocal8Bit("芯片名称：");
					list.append(str+query.value(3).toString());
					str = str.fromLocal8Bit("规格：");
					list.append(str+query.value(4).toString());
					str = str.fromLocal8Bit("封装：");
					list.append(str+query.value(5).toString());
					str = str.fromLocal8Bit("额定电压：");
					list.append(str+query.value(6).toString());
					str = str.fromLocal8Bit("额定电流：");
					list.append(str+query.value(7).toString());
					str = str.fromLocal8Bit("抗辐射指标：");
					list.append(str+query.value(8).toString());
				}
				/*this->ui.listWidget->addItems(list);*/
			}

		};

	}
	else
	{
		//		ui.tableView->setVisible(true);
		ui.comboBox->setEnabled(true);
		/*onChanged();*/
	}
}
void Advancedsearch::setCompleter(const QString &text) {
	namelistview->hide();
	//disconnect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
	if (text.isEmpty()) {
		namelistview->hide();
		return;
	}

	if ((text.length() > 1) && (!namelistview->isHidden())) {
		return;
	}

	QSqlQuery query(*sql.db);	

	
	QString strsql= "select * from PARAMETER where SATELLITENO like '"+text+"'";
	query.exec(strsql);
	if (query.next())return;

	QStringList list;
	for(int i = 0;i < vtSat.size();++i){

		if (vtSat[i].indexOf(text) != -1)
		{

			list.append(vtSat[i]);
		}
	}

	model->setStringList(list);
	//	model->setStringList(sl);
	namelistview->setModel(model);

	if (model->rowCount() == 0) {
		return;
	}

	// Position the text edit
	namelistview->setMinimumWidth(this->width());
	namelistview->setMaximumWidth(this->width());

	QPoint p(0, this->height());
	int x = this->mapToGlobal(p).x();
	int y = this->mapToGlobal(p).y() + 1;

	//listView->move(x, y);
	namelistview->setGeometry(this->x()+60, this->y()+100, 50, 100);
	namelistview->resize(100,200);
	namelistview->setFixedWidth(123);
	namelistview->show();
//	connect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
}
void Advancedsearch::completeText(const QModelIndex &index) {
	QString strName = index.data().toString();
	for (int i =0;i<ui.comboBox->count();i++)
	{
		if (ui.comboBox->itemText(i)==strName)
		{
			ui.comboBox->setCurrentIndex(i);
		}
	}
	//ui.comboBox->setEditText(strName);
	namelistview->hide();
}
void Advancedsearch::on_startdateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.startdateEdit->setText(dateDlg->strDate);;
	}
}
void Advancedsearch::on_enddateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.enddateEdit->setText(dateDlg->strDate);;
	}
}
void Advancedsearch::keyPressEvent(QKeyEvent *e) {
	if (!namelistview->isHidden()) {
		int key = e->key();
		int count = namelistview->model()->rowCount();
		QModelIndex currentIndex = namelistview->currentIndex();

		if (Qt::Key_Down == key) {
			// 按向下方向键时，移动光标选中下一个完成列表中的项
			int row = currentIndex.row() + 1;
			if (row >= count) {
				row = 0;
			}

			QModelIndex index = namelistview->model()->index(row, 0);
			namelistview->setCurrentIndex(index);
		} else if (Qt::Key_Up == key) {
			// 按向下方向键时，移动光标选中上一个完成列表中的项
			int row = currentIndex.row() - 1;
			if (row < 0) {
				row = count - 1;
			}

			QModelIndex index = namelistview->model()->index(row, 0);
			namelistview->setCurrentIndex(index);
		} else if (Qt::Key_Escape == key) {
			// 按下Esc键时，隐藏完成列表
			namelistview->hide();
		} else if (Qt::Key_Enter == key || Qt::Key_Return == key) {
			// 按下回车键时，使用完成列表中选中的项，并隐藏完成列表
			if (currentIndex.isValid()) {
				QString text = namelistview->currentIndex().data().toString();
				ui.comboBox->setItemText(0,text);
			}

			namelistview->hide();
		} else {
			// 其他情况，隐藏完成列表，并使用QLineEdit的键盘按下事件
			namelistview->hide();
			//QLineEdit::keyPressEvent(e);
		}
	} else {
		//QLineEdit::keyPressEvent(e);
	}
}
Advancedsearch::~Advancedsearch()
{

}
