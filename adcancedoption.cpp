#include "adcancedoption.h"
#include "connectsql.h"
extern ConnectSql sql;

Adcancedoption::Adcancedoption(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	setStyleSheet("QLineEdit{border-radius: 0px;}");
	QRegExp regExp("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");//限制文本框输入
	ui.startLatitudeEdit->setValidator(new QRegExpValidator(regExp, this));
	ui.startLongitudeEdit->setValidator(new QRegExpValidator(regExp, this));
	ui.endLatitudeEdit->setValidator(new QRegExpValidator(regExp, this));
	ui.endLongitudeEdit->setValidator(new QRegExpValidator(regExp, this));
	ui.longitudeIDEdit->setValidator(new QRegExpValidator(regExp, this));
	ui.latitudeIDEdit->setValidator(new QRegExpValidator(regExp, this));
	connect(ui.radioButton, SIGNAL(clicked(bool)), this, SLOT(setChoice(bool)));
	connect(ui.radioButton_2, SIGNAL(clicked(bool)), this, SLOT(setChoice2(bool)));
	connect(ui.checkBox,SIGNAL(stateChanged(int)),this,SLOT(siftPositionCheck()));
	connect(ui.checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(siftIDCheck()));
}
void Adcancedoption::initUI()
{
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());

	}
	ui.listWidget->addItems(list);

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
	namelist->append(QString::fromLocal8Bit("单错次数"));	
	namelist->append(QString::fromLocal8Bit("发生错误的IO或RAM或ROM地址"));	
	namelist->append(QString::fromLocal8Bit("陷阱类型"));	
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
	namelist->append(QString::fromLocal8Bit("NOR_FLASH双错累计次数"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH单错累计次数"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH错误计数"));	
	namelist->append(QString::fromLocal8Bit("同步故障类型"));	

	QHBoxLayout *hlayout = new QHBoxLayout;
	QComboBox*namecombox= new QComboBox;
	QComboBox*siftcombox= new QComboBox;

	namecombox->addItems(*namelist);
	siftcombox->addItems(*siftlist);
	ui.listWidget_2->addItems(*namelist);
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
	ui.radioButton->setChecked(true);
}
//平闰年的判断// 
int Adcancedoption:: leap(int year) 
{ 
	if ( year%4==0&&year%100!=0||year % 400 ==0) 
		return 1; 
	else
		return 0; 
}
//计算天数// 
int Adcancedoption::count_day (int year, int month, int day, int flag) 
{ 
	static int day_tab[2][12] = {{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};
	//二维数组存放天数
	int p, i, s; 
	if (leap(year))                                                                                //闰年为1选择后面的用
		p = 1; 
	else   //平年为0选择前面的用
		p = 0; 
	if (flag) 
	{ 
		s = day; 
		for ( i = 1; i < month; i++ ) 
			s += day_tab[p][i-1];//查询数组累加天数
	} 
	else
	{ 
		s = day_tab[p][month] - day; 
		for ( i = month + 1; i <= 12; i++ ) 
			s += day_tab[p][i-1]; 
	} 
	return ( s );//返回天数
} 
int Adcancedoption::delta_day(int startYear,int startMonth,int startDay,int endYear,int endMonth,int endDay)
{

	long deltadate,year, day1, day2; 
	if ( startYear < endYear ) 
	{ 
		deltadate = count_day ( startYear, startMonth, startDay , 0 ); 
		for (year=startYear+1;year<endYear; year++ )//年份累加
			if (leap(year))//遇到闰年
				deltadate += 366; 
			else   //平年
				deltadate += 365; 
		deltadate += count_day (endYear, endMonth, endDay, 1); 

	} 
	//同一年的时间判断
	else if ( startYear == endYear ) 
	{ 
		day1 = count_day ( startYear, startMonth, startDay, 1 ); 
		day2 = count_day ( endYear, endMonth, endDay, 1 ); 
		deltadate = day2-day1;//多的被减出来了
	} 
	else
		deltadate = -1; //标识
	return deltadate;
}
void Adcancedoption::on_addButton_3_clicked()
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
	if (icount>8)
	{
		this->setFixedHeight(this->height()+50);
		QRect rect(240,30,290,60*icount);
		ui.verticalLayout->setGeometry(rect);
	}
}
void Adcancedoption::on_okButton_clicked()
{
	int icount = lineedits.count();
	QString strsql;
	QStringList sqllist;
	int satcount = ui.selectlistWidget->count();
	QString startDate=ui.startdateEdit->text();//ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString endDate=ui.enddateEdit->text();//ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	

	startdatetime =QDateTime::fromString(startDate, "yyyy-MM-dd hh:mm:ss");  // ui.dateTimeEdit->dateTime();
	enddatetime = QDateTime::fromString(endDate, "yyyy-MM-dd hh:mm:ss"); //ui.dateTimeEdit_2->dateTime();
	//时间处理
	startYear=startdatetime.date().year();
	startMonth=startdatetime.date().month();
	startDay=startdatetime.date().day();

	int endYear=enddatetime.date().year();
	int endMonth=enddatetime.date().month();
	int endDay=enddatetime.date().day();

	deltadate =startdatetime.daysTo(enddatetime);//delta_day(startYear,startMonth,startDay,endYear,endMonth,endDay);
	deltasecs =startdatetime.secsTo(enddatetime);
	timeID= ui.comboBox_2->currentIndex();

	if (ui.startLongitudeEdit->text()==""||ui.startLatitudeEdit->text()==""||ui.endLongitudeEdit->text()==""||ui.endLatitudeEdit->text()=="")
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请给出区域范围"));
		return;
	}
	double startPointLongitude = ui.startLongitudeEdit->text().toDouble();
	double startPointLatitude = ui.startLatitudeEdit->text().toDouble();

	double endPointLongitude = ui.endLongitudeEdit->text().toDouble();
	double endPointLatitude = ui.endLatitudeEdit->text().toDouble();
	if ((startPointLongitude<-180||startPointLongitude>180)||(endPointLongitude<-180||endPointLongitude>180))
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("经度值应该在-180与180之间"));
		return;
	}
	if (startPointLatitude<-90||startPointLatitude>90||endPointLatitude<-90||endPointLatitude>90)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("纬度值应该在-90与90之间"));
		return;
	}


	//经纬度范围初始化
	this->LongitudeCount = ui.longitudeBox->currentText().toDouble();
	this-> LatitudeCount = ui.latitudeBox->currentText().toDouble();
	double UnitLongitude = (double)360/LongitudeCount;
	double UnitLatitude = (double)180/LatitudeCount;

	//double startPointLongitude = ui.startLongitudeEdit->text().toDouble();
	//double startPointLatitude = ui.startLatitudeEdit->text().toDouble();

	//double endPointLongitude = ui.endLongitudeEdit->text().toDouble();
	//double endPointLatitude = ui.endLatitudeEdit->text().toDouble();

	if (startPointLongitude<0)
	{
		startPointLongitude+=360;
	}
	if (endPointLongitude<=0)
	{
		endPointLongitude+=360;
	}
	if (startPointLongitude>endPointLongitude&&startPointLatitude<endPointLatitude)
	{
		double temp=startPointLongitude;
		startPointLongitude=endPointLongitude;
		endPointLongitude=temp;
		temp=startPointLatitude;
		startPointLatitude=endPointLatitude;
		endPointLatitude=temp;
	}
	else if (startPointLongitude<endPointLongitude&&startPointLatitude<endPointLatitude)
	{
		double temp=startPointLatitude;
		startPointLatitude=endPointLatitude;
		endPointLatitude=temp;
	}
	else if (startPointLongitude>endPointLongitude&&startPointLatitude>endPointLatitude)
	{
		double temp=startPointLongitude;
		startPointLongitude=endPointLongitude;
		endPointLongitude=temp;
	}
	//经纬度编号初始化
	int longitudeID = ui.longitudeIDEdit->text().toInt();
	int latitudeID= ui.latitudeIDEdit->text().toInt();
	if(IssiftID==true)
	{
		startPointLongitude=UnitLongitude*longitudeID;
		startPointLatitude=90-UnitLatitude*latitudeID;
		endPointLongitude=UnitLongitude*(longitudeID+1);
		endPointLatitude=90-UnitLatitude*(latitudeID+1);
	}
	QString strlongitude = QString::fromLocal8Bit("经度");
	QString strlatitude = QString::fromLocal8Bit("纬度");
	sqllist.clear();

	int paracount = ui.selectlistWidget_2->count();
	int satellitecount = ui.selectlistWidget->count();
	if (satellitecount==0)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请选择卫星"));
		return;
	}
	if (paracount==0)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请选择参数数据"));
		return;
	}
	if(!Ismultisat)
	{
		QString strSat = ui.selectlistWidget->item(0)->text();
		satlist.append(strSat);
		for(int j=0;j<paracount;j++)
		{
			QString strpara = ui.selectlistWidget_2->item(j)->text();
			paralist.append(strpara);

			if (sql.dboracle==true)
			{
				strsql = QString("select  %1 , %2 ,  %3 ,DATETIME from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'").arg(strpara).arg(strlongitude).arg(strlatitude);
			}
			else
			{
				strsql =  QString("select  %1 , %2 ,  %3 ,DATETIME  from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# ").arg(strpara).arg(strlongitude).arg(strlatitude);
			}


			if (IssiftID||IssiftPosition)
			{
				strsql+=QString("and %1 >%2 and   %1 <%3 and %4  <%5 and  %4 >%6").arg(strlongitude).arg(startPointLongitude).arg(endPointLongitude).arg(strlatitude).arg(startPointLatitude).arg(endPointLatitude);
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
			if (strsql!="")
			{
				//strsql +="order by ID";
				sqllist.append(strsql);

			}
		}
	}
	else
	{
		QString strpara = ui.selectlistWidget_2->item(0)->text();
		paralist.append(strpara);
		for (int i =0;i<satcount;i++)
		{
			QString strSat = ui.selectlistWidget->item(i)->text();
			satlist.append(strSat);
		
			if (sql.dboracle==true)
			{
				strsql = QString("select  %1 , %2 ,  %3 ,DATETIME from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'").arg(strpara).arg(strlongitude).arg(strlatitude);
			}
			else
			{
				strsql =  QString("select  %1 , %2 ,  %3 ,DATETIME  from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# ").arg(strpara).arg(strlongitude).arg(strlatitude);
			}

			//if (sql.dboracle==true)
			//{
			//	strsql = QString("select  %1 ,经度, 纬度,DATETIME from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'").arg(strpara);
			//}
			//else
			//{
			//	strsql =  QString("select  %1 ,经度, 纬度,DATETIME  from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# ").arg(strpara);
			//}
	
			if (IssiftID||IssiftPosition)
			{
				strsql+=QString("and %1 >%2 and   %1 <%3 and %4  <%5 and  %4 >%6").arg(strlongitude).arg(startPointLongitude).arg(endPointLongitude).arg(strlatitude).arg(startPointLatitude).arg(endPointLatitude);
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
			if (strsql!="")
			{
				//strsql +="order by ID";
				sqllist.append(strsql);
			
			}
		}
	}
	if (IssiftID==true)
	{
		strzone=QString::fromLocal8Bit("经纬度编号：")+QString::number(longitudeID)+","+QString::number(latitudeID); 
	}
	else if(IssiftPosition==true)
	{
		strzone= QString::fromLocal8Bit("经纬度区域：")+QString::number(startPointLongitude)+","+QString::number(startPointLatitude)+";"+QString::number(endPointLongitude)+","+QString::number(endPointLatitude); 
	}
	else 
	{
		strzone= QString::fromLocal8Bit("全球");
	}
	emit getsql(sqllist);
	this->hide();
}
QString Adcancedoption::getsift(QString strsift)
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
void Adcancedoption::on_cancelButton_clicked()
{
	this->close();
}
void  Adcancedoption::on_addButton_clicked()
{
	int currentcount=ui.selectlistWidget->count();
	if (ui.radioButton_2->isChecked())
	{
		if(currentcount>0) return;
	}
	QList<QListWidgetItem*> list=ui.listWidget->selectedItems();
	for (int i=0;i<list.count();i++)
	{
		for (int j=0;j<currentcount;j++)
		{
			if (ui.selectlistWidget->item(j)->text()==list[i]->text())
				return;
		}
		if (ui.radioButton_2->isChecked())
		{
			if(i==1) return;
		}
		ui.selectlistWidget->addItem(list[i]->text());
	}
}
void  Adcancedoption::on_delButton_clicked()
{
	QList<QListWidgetItem*> list= ui.selectlistWidget->selectedItems();
	for (int i=0;i<list.count();i++)
	{
		ui.selectlistWidget->takeItem(list[i]->data(0).toInt());
	}//list.takeAt(i).data().toInt()
}
void  Adcancedoption::on_addButton_2_clicked()
{
	int currentsatcount=ui.selectlistWidget->count();
	if (currentsatcount<1)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请先选择卫星"));
		return;
	}
	int currentcount=ui.selectlistWidget_2->count();
	if (ui.radioButton->isChecked())
	{
		if(currentcount>0) return;
	}
	QList<QListWidgetItem*> list=ui.listWidget_2->selectedItems();
	for (int i=0;i<list.count();i++)
	{
		for (int j=0;j<currentcount;j++)
		{
			if (ui.selectlistWidget_2->item(j)->text()==list[i]->text())
			continue;
		}
		if (ui.radioButton->isChecked())
		{
			if(i==1) 
			{
				QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("当前只允许选择一组参数"));
				return;
			}
		}
		ui.selectlistWidget_2->addItem(list[i]->text());
	}
}
void  Adcancedoption::on_delButton_2_clicked()
{
	QList<QListWidgetItem*> list= ui.selectlistWidget_2->selectedItems();
	for (int i=0;i<list.count();i++)
	{
		ui.selectlistWidget_2->takeItem(list[i]->data(0).toInt());
	}//list.takeAt(i).data().toInt()
}
void Adcancedoption::setChoice(bool Issat)
{
	Ismultisat = Issat;
	/*if (Issat)
	{
	ui.listWidget_2->setEnabled(false);
	ui.selectlistWidget_2->setEnabled(false);
	ui.addButton_2->setEnabled(false);
	ui.delButton_2->setEnabled(false);
	ui.listWidget->setEnabled(true);
	ui.selectlistWidget->setEnabled(true);
	ui.addButton->setEnabled(true);
	ui.delButton->setEnabled(true);
	}
	else
	{
	ui.listWidget_2->setEnabled(true);
	ui.selectlistWidget_2->setEnabled(true);
	ui.addButton_2->setEnabled(true);
	ui.delButton_2->setEnabled(true);
	}*/
}
void Adcancedoption::setChoice2(bool Issat)
{
	Ismultisat = !Issat;
	/*if (Issat)
	{
	ui.listWidget->setEnabled(false);
	ui.selectlistWidget->setEnabled(false);
	ui.addButton->setEnabled(false);
	ui.delButton->setEnabled(false);
	ui.listWidget_2->setEnabled(true);
	ui.selectlistWidget_2->setEnabled(true);
	ui.addButton_2->setEnabled(true);
	ui.delButton_2->setEnabled(true);
	}
	else
	{
	ui.listWidget->setEnabled(true);
	ui.selectlistWidget->setEnabled(true);
	ui.addButton->setEnabled(true);
	ui.delButton->setEnabled(true);
	}*/
}
void Adcancedoption::siftPositionCheck()
{
	if(IssiftID==true) return;
	if (ui.checkBox->checkState()==Qt::Checked)
	{
	ui.startLongitudeEdit->setEnabled(true);
	ui.startLatitudeEdit->setEnabled(true);
	ui.endLongitudeEdit->setEnabled(true);
	ui.endLatitudeEdit->setEnabled(true);
	ui.checkBox_2->setEnabled(false);
	IssiftPosition=true;
	}
	else
	{
	ui.startLongitudeEdit->setEnabled(false);
	ui.startLatitudeEdit->setEnabled(false);
	ui.endLongitudeEdit->setEnabled(false);
	ui.endLatitudeEdit->setEnabled(false);
	ui.checkBox_2->setEnabled(true);
	IssiftPosition=false;
	}
}
void Adcancedoption::siftIDCheck()
{
	if(IssiftPosition==true) return;
	if (ui.checkBox_2->checkState()==Qt::Checked)
	{
		ui.checkBox->setEnabled(false);
		ui.longitudeIDEdit->setEnabled(true);
		ui.latitudeIDEdit->setEnabled(true);
		IssiftID=true;
	}
	else
	{
		ui.checkBox->setEnabled(true);
		ui.longitudeIDEdit->setEnabled(false);
		ui.latitudeIDEdit->setEnabled(false);
		IssiftID=false;
	}
}
void Adcancedoption::on_startdateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.startdateEdit->setText(dateDlg->strDate);;
	}
}
void Adcancedoption::on_enddateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.enddateEdit->setText(dateDlg->strDate);;
	}
}

Adcancedoption::~Adcancedoption()
{

}
