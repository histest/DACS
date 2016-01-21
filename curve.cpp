#include "curve.h"
#include <QGridLayout>
#include <QLinearGradient>
#include "connectsql.h"
#include "multisatdlg.h"
#include "Tools/disEarth.h"
#include "Tools/UnitConstant.h"
#include "Tools/UserMath.h"
#include "Tools/UserMechanics.h"
#include "Tools/UnitConstant.h"
#include "odbcexcel.h"
#include <QDesktopServices>
#include"mycutimagewidget.h"
#include <QDesktopWidget>
#include <string>
#include <QTimer>
#include "mydefine.h"
#include <QTextCodec>
#include <vector>  
using namespace std;
extern bool Isfresh;
extern ConnectSql sql;
vector <QString> vecSat; 
curve::curve(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	connect(ui.linetypeBox,SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));
	connect(ui.comboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(Changed()));
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 0.5px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QLineEdit{border-radius: 0px;}"
		"QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");
	namelistview = new QListView(this);
	model = new QStringListModel(this);
	namelistview->setWindowFlags(Qt::ToolTip);
	installEventFilter(namelistview);
	connect(namelistview, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeText(const QModelIndex &)));
	connect(ui.comboBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(setCompleter(const QString &)));
	IsStop=false;
}
void curve::initUI()
{
	vecSat.clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList list;
	bool Isexist=false;
	while(query.next())
	{
		for (int i=0;i<list.count();i++)
		{
			if (query.value(1).toString()==list.at(i))
				Isexist=true;
		}
		if(!Isexist)
		{
			list.append(query.value(1).toString());
			vecSat.push_back(query.value(1).toString());
		}
		Isexist=false;
	}
	this->ui.comboBox->addItems(list);
	Changed();	
	satellitecount =0;

	QStringList header;  
	header<<QString::fromLocal8Bit("卫星")<<QString::fromLocal8Bit("时间")<<QString::fromLocal8Bit("单错次数")<<QString::fromLocal8Bit("占总数百分比")<<QString::fromLocal8Bit("区域范围"); 
	ui.tableWidget->setColumnCount(5);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//ui.startLongitudeEdit->setEnabled(false);
	//ui.startLatitudeEdit->setEnabled(false);
	//ui.endLongitudeEdit->setEnabled(false);
	//ui.endLatitudeEdit->setEnabled(false);
	IssiftPosition=false;
	IssiftID=false;

	QStringList*namelist = new QStringList;
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
	ui.comboBox_3->addItems(*namelist);
	QDateTime now = QDateTime::currentDateTime();
	QString endtime = now.toString("yyyy-MM-dd hh:mm:ss");  
	ui.enddateEdit->setText(endtime);

	QDateTime start = now.addYears(-1);
	QString starttime = start.toString("yyyy-MM-dd hh:mm:ss");  
	ui.startdateEdit->setText(starttime);
}
void curve::refresh()
{
	this->ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList list;
	bool Isexist=false;
	while(query.next())
	{
		for (int i=0;i<list.count();i++)
		{
			if (query.value(1).toString()==list.at(i))
				Isexist=true;
		}
		if(!Isexist)
		{
			list.append(query.value(1).toString());
			vecSat.push_back(query.value(1).toString());
		}
		Isexist=false;
	}
	this->ui.comboBox->addItems(list);
}
void curve::setupChart()
{
	this->moveToThread(&displayThread);
	static int day_tab[2][12] = {{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};
	int xcount,remainday,remainsecs;
	double totalcount=0;
	int index = this->timeID;//时间单位

	if (deltadate>3650&&index==2)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("时间跨度太大，请以月或年为单位"));
		return;
	}

	//if (index==2)
	//{
	//	xcount=deltadate/7;
	//	remainday=deltadate%7;
	//}
	if (index==0)
	{
		xcount=deltadate/31;
		remainday=deltadate%31;
	}
	if (index==1)
	{
		xcount=deltadate/365;
		remainday=deltadate%365;
	}

	if (index==2)
	{
		xcount=deltadate/1;
		remainday=deltadate%1;
		if(xcount>1000)
		{
			QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("时间跨度太大，请以月或年为单位"));
			return;
		}
	}
	if (index==3)
	{
		xcount=deltasecs/3600;
		remainsecs=deltasecs%3600;
		if(xcount<24000&&xcount>1000)
		{
			QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("时间跨度太大，请以天或月为单位"));
		return;
		}
	}
	if (index==4)
	{
		xcount=deltasecs/60;
		remainsecs=deltasecs%60;
		if(xcount<60000&&xcount>1000)
		{
			QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("时间跨度太大，请以时或天为单位"));
			return;
		}
	}
	if (index==5)
	{
		xcount=deltasecs/1;
		remainsecs=deltasecs%1;
		if(xcount>1000)
		{
			QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("时间跨度太大，请以时或分为单位"));
			return;
		}
	}

	if (IsBarchart==true)
	{
		chart = new XYChart(900, 500);
	}
	else
	{
		chart = new XYChart(900, 500);
	}
	chart->setDefaultFonts("simsun.ttc");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	if(IsBarchart==true)
	{
		chart->setPlotArea(50, 50, 800, 410, 0xf8f8f8, 0xffffff);
		chart->addLegend(300, 25, false, "simsun.ttc", 10)->setBackground(Chart::Transparent);
		chart->xAxis()->setTickOffset(0.5);
		const char *labels[256] = {};
		chart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
	}
	else
	{
		chart->setPlotArea(50, 50, 921, 390, chart->linearGradientColor(0, 55, 0, 335, 0xf9fcff, 0xaaccff), -1,
			Chart::Transparent, 0xffffff);
		chart->addLegend(300, 25, false, "simsun.ttc", 10)->setBackground(Chart::Transparent);
		const char *labels[256] = {};
		chart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
	}
	BarLayer *barlayer = chart->addBarLayer(Chart::Side, 0);
	satellitecount = sqllist.count(); 
	int rowcount=0;
	rowcount=(xcount+1)*satellitecount;
	ui.tableWidget->setRowCount(rowcount);


	for (int i=0;i<satellitecount;i++)
	{
		QSqlQuery query(*sql.db);
		QString strSat,strPara;
		if (IsMultisat)
		{
			strSat	=satlist.at(i);
			satelliteNo[i] = strSat;
			strPara = paralist.at(0);
		}
		if (!IsMultisat)
		{
			strSat	=satlist.at(0);
			satelliteNo[i] = paralist.at(i);
			strPara = paralist.at(i);
		}

		int k=0;
		int j=0;
		int m=0;
		double x[1024]={0.0};
		double y[1024]={0.0};
		int dbdeltaDay=0;
		int deltaDay=0;
		int dbdeltasecs=0;
		int deltasecs=0;
		double count=0;
		double totalcount=0;

		QDateTime lastDateTime;
		int lastYear=0,lastMonth=0,lastDay=0,lastDate=0;
		QStringList info;
		int lastDeltaDay=0;
		int lastDeltasecs=0;
		query.exec(sqllist.at(i));
		while(query.next())
		{
			QDateTime datetime = query.value(3).toDateTime();
			int currentYear = datetime.date().year();
			int currentMonth =datetime.date().month();
			int currentDay = datetime.date().day();
			if(j>0)
			{
				dbdeltaDay+=lastDateTime.daysTo(datetime);//option->delta_day(lastYear,lastMonth,lastDay,currentYear,currentMonth,currentDay);
				dbdeltasecs += lastDateTime.secsTo(datetime);
			}
			lastDay = currentDay;	
			lastMonth = currentMonth;
			lastYear=currentYear;
			lastDateTime = datetime;
			dbdeltaDay=startdatetime.daysTo(datetime);
			j++;
		}
		if (index==1)
		{
			remainday =dbdeltaDay%31;
		}
		query.exec(sqllist.at(i));
		while(query.next())
		{
			QCoreApplication::processEvents();
			//计算与开始日期的日差
			QDateTime datetime = query.value(3).toDateTime();
			int currentYear = datetime.date().year();
			int currentMonth =datetime.date().month();
			int currentDay = datetime.date().day();
			deltaDay=startdatetime.daysTo(datetime);//option->delta_day(startYear,startMonth,startDay,currentYear,currentMonth,currentDay);
			deltasecs = startdatetime.secsTo(datetime);
			count+=query.value(0).toDouble();//
			m++;

			//if (index==2)
			//{
			//	
			//	if(k!=xcount)
			//	{
			//		if (deltaDay%7!=0||deltaDay==0)
			//		{
			//			y[deltaDay/7+1]+=count;
			//			x[deltaDay/7+1]=deltaDay/7+1;	
			//			totalcount+=count;
			//			count=0;
			//		}
			//		if (deltaDay%7==0&&deltaDay!=0)
			//		{
			//			//	data0[k]=count;
			//			y[deltaDay/7]+=count;
			//			x[deltaDay/7]=k;	
			//			totalcount+=count;
			//			if(deltaDay!=lastDeltaDay) k++;
			//			count=0;
			//			lastDeltaDay=deltaDay;
			//		}
			//	}
			//	else
			//	{
			//		y[deltaDay/7+1]+=count;
			//		x[deltaDay/7+1]=deltaDay/7+1;	
			//		totalcount+=count;
			//		count=0;
			//		if (deltaDay==remainday)
			//		{
			//			//	data0[k]=count;
			//			y[k+1]=count;
			//			x[k+1]=k;	
			//			deltaDay=0;
			//			totalcount+=count;
			//			count=0;
			//			k++;
			//		}
			//	}


			////}
			//else
			if(index==0) 
			{
				if(k!=xcount)
				{
					if ((deltaDay)%31!=0||deltaDay==0)
					{
						y[deltaDay/31+1]+=count;
						x[deltaDay/31+1]=deltaDay/31+1;	
						totalcount+=count;
						count=0;
					}
					if ((deltaDay)%31==0&&deltaDay!=0)
					{
						y[deltaDay/31]+=count;
						x[deltaDay/31]=k;	
						totalcount+=count;
						if(deltaDay!=lastDeltaDay) k++;
						count=0;
						lastDeltaDay=deltaDay;
					}
					if (deltaDay==dbdeltaDay&&xcount!=0)
					{
						y[k+1]+=count;
						x[k+1]=k;	
						deltaDay=0;
						totalcount+=count;
						count=0;
						k++;
					}
				}
				else
				{
					y[(deltaDay+1)/31+1]+=count;
					x[(deltaDay+1)/31+1]=deltaDay/31+1;	
					totalcount+=count;
					count=0;
					if (deltaDay==remainday&&xcount!=0)
					{
						//data0[k]=count;
						y[k+1]=count;
						x[k+1]=k;	
						totalcount+=count;
						if(deltaDay!=lastDeltaDay) k++;
						count=0;
						lastDeltaDay=deltaDay;
					}
				}		
				//if(k!=xcount)
				//{
				//	if ((deltaDay)%31!=0||deltaDay==0)
				//	{
				//		y[deltaDay/31+1]+=count;
				//		x[deltaDay/31+1]=deltaDay/31+1;	
				//		totalcount+=count;
				//		count=0;
				//	}
				//	if ((deltaDay)%31==0&&deltaDay!=0)
				//	{
				//		//data0[k]=count;
				//		y[deltaDay/31]+=count;
				//		x[deltaDay/31]=k;	
				//		totalcount+=count;
				//		if(deltaDay!=lastDeltaDay) k++;
				//		count=0;
				//		lastDeltaDay=deltaDay;
				//	}
				//	if (deltaDay==dbdeltaDay&&xcount!=0)
				//	{
				//		y[k+1]+=count;
				//		x[k+1]=k;	
				//		deltaDay=0;
				//		totalcount+=count;
				//		count=0;
				//		k++;
				//	}
				//}
				//else
				//{
				//	y[(deltaDay+1)/31+1]+=count;
				//	x[(deltaDay+1)/31+1]=deltaDay/31+1;	
				//	totalcount+=count;
				//	count=0;
				//	if (deltaDay==remainday&&xcount!=0)
				//	{
				//		//data0[k]=count;
				//		y[k+1]=count;
				//		x[k+1]=k;	
				//		totalcount+=count;
				//		if(deltaDay!=lastDeltaDay) k++;
				//		count=0;
				//		lastDeltaDay=deltaDay;
				//	}
				//}
			}
			else if(index==1) 
			{
				if (deltaDay%365!=0||deltaDay==0)
				{
					y[deltaDay/365+1]+=count;
					x[deltaDay/365+1]=deltaDay/365+1;	
					totalcount+=count;
					count=0;
				}
				if(k!=xcount)
				{
					if (deltaDay%365==0&&deltaDay!=0)
					{
						y[deltaDay/365]+=count;
						x[deltaDay/365]=k;	
						totalcount+=count;
						if(deltaDay!=lastDeltaDay) k++;
						count=0;
						lastDeltaDay=deltaDay;
					}
					if (deltaDay==dbdeltaDay&&xcount!=0)
					{
						y[k+1]+=count;
						x[k+1]=k;	
						deltaDay=0;
						totalcount+=count;
						count=0;
						k++;
					}
				}
				else
				{
					y[deltaDay/365+1]+=count;
					x[deltaDay/365+1]=deltaDay/365+1;	
					totalcount+=count;
					count=0;
					if (deltaDay==remainday&&xcount!=0)//
					{
						//data0[k]=count;
						y[k+1]=count;
						x[k+1]=k;	
						deltaDay=0;
						totalcount+=count;
						count=0;
						k++;
					}

				}
			}

			if(index==2) 
			{
				y[deltaDay]+=count;
				x[deltaDay]=deltaDay;	
				totalcount+=count;
				count=0;

			}
			if(index==3) 
			{
				y[deltasecs/3600]+=count;
				x[deltasecs/3600]=deltasecs/3600;	
				totalcount+=count;
				count=0;

			}
			if(index==4) 
			{
				y[deltasecs/60]+=count;
				x[deltasecs/60]=deltasecs/60;	
				totalcount+=count;
				count=0;

			}
			if(index==5) 
			{
				y[deltasecs/1]+=count;
				x[deltasecs/1]=deltasecs/1;	
				totalcount+=count;
				count=0;

			}

		}
		if(m==0&&satellitecount==1)
		{
			QMessageBox::information(this,QString::fromLocal8Bit("提示"),satelliteNo[i]+QString::fromLocal8Bit("在该时间段内无数据"));
			return;
		}
		for (int j=0;j<xcount+2;j++)
		{		
			int currentday = this->startdatetime.date().day()+j;
			QDateTime currentDateTime;
			QDate currentDate;
			ui.tableWidget->setItem(j+i*(xcount+1),0,new QTableWidgetItem(satelliteNo[i])); 
			if (index==2)
			{
				currentDate = this->startdatetime.date().addDays(j);
				QString strDate = currentDate.toString("yyyy-MM-dd");
				ui.tableWidget->setItem(j+i*(xcount+1),1,new QTableWidgetItem(strDate));// QString::fromLocal8Bit("第")+QString::number(j+1)+QString::fromLocal8Bit("天")			

			}
			else if(index==1)
			{
				currentDate = this->startdatetime.date().addYears(j);
				QString strDate = currentDate.toString("yyyy");
				ui.tableWidget->setItem(j+i*(xcount+1),1,new QTableWidgetItem(strDate));//new QTableWidgetItem(QString::fromLocal8Bit("第")+QString::number(j+1)+QString::fromLocal8Bit("月"))); 
			}
			else if(index==0)
			{
				currentDate = this->startdatetime.date().addMonths(j);
				QString strDate = currentDate.toString("yyyy-MM");
				ui.tableWidget->setItem(j+i*(xcount+1),1,new QTableWidgetItem(strDate));//new QTableWidgetItem(QString::fromLocal8Bit("第")+QString::number(j+1)+QString::fromLocal8Bit("年"))); 
			}
			else if(index==3)
			{			
				QTime time =this->startdatetime.time().addSecs(j*3600);
				QString strTime = time.toString("hh:mm:ss");
				currentDate = this->startdatetime.date().addDays(j/24);
				QString strDate = currentDate.toString("yyyy-MM-dd");
				strDate+=" "+strTime;
				ui.tableWidget->setItem(j+i*(xcount+1),1,new QTableWidgetItem(strDate));//,new QTableWidgetItem(QString::fromLocal8Bit("第")+QString::number(j+1)+QString::fromLocal8Bit("小时"))); 
			}
			else if(index==4)
			{
				QTime time =this->startdatetime.time().addSecs(j*60);
				QString strTime = time.toString("hh:mm:ss");
				currentDate = this->startdatetime.date().addDays(j/1440);
				QString strDate = currentDate.toString("yyyy-MM-dd");
				strDate+=" "+strTime;
				ui.tableWidget->setItem(j+i*(xcount+1),1,new QTableWidgetItem(strDate));//,new QTableWidgetItem(QString::fromLocal8Bit("第")+QString::number(j+1)+QString::fromLocal8Bit("分钟"))); 
			}
			else if(index==5)
			{
				QTime time =this->startdatetime.time().addSecs(j);
				QString strTime = time.toString("hh:mm:ss");
				currentDate = this->startdatetime.date().addDays(j/86400);
				QString strDate = currentDate.toString("yyyy-MM-dd");
				strDate+=" "+strTime;
				ui.tableWidget->setItem(j+i*(xcount+1),1,new QTableWidgetItem(strDate));//,new QTableWidgetItem(QString::fromLocal8Bit("第")+QString::number(j+1)+QString::fromLocal8Bit("秒"))); 
			}
			ui.tableWidget->setItem(j+i*(xcount+1),2,new QTableWidgetItem(QString::number(y[j+1]))); 
			if(totalcount>0)
				ui.tableWidget->setItem(j+i*(xcount+1),3,new QTableWidgetItem(QString::number(y[j+1]*100/totalcount)+"%")); 
			ui.tableWidget->setItem(j+i*(xcount+1),4,new QTableWidgetItem(strzone)); 
			/*if (IssiftID==true)
			{
				ui.tableWidget->setItem(j+i*(xcount+1),4,new QTableWidgetItem(QString::fromLocal8Bit("经纬度编号：")+QString::number(longitudeID)+","+QString::number(latitudeID))); 
			}
			else if(IssiftPosition==true)
			{
				ui.tableWidget->setItem(j+i*(xcount+1),4,new QTableWidgetItem(QString::fromLocal8Bit("经纬度区域：")+QString::number(startPointLongitude)+","+QString::number(startPointLatitude)+";"+QString::number(endPointLongitude)+","+QString::number(endPointLatitude))); 
			}
			else
			{
				ui.tableWidget->setItem(j+i*(xcount+1),4,new QTableWidgetItem(QString::fromLocal8Bit("全球"))); 
			}*/
		}

		if(IsBarchart==true)
		{
			//std::string strtemp = satelliteNo[i].toStdString();
			std::string strtemp = QTextCodec::codecForName("UTF-8")->fromUnicode(satelliteNo[i]).constData();
		//	BarLayer *barlayer = chart->addBarLayer();

			double data[1];
			if(xcount==0)
			{
				data[0]=y[1];

				if((i+1)%2==0)
				{
					barlayer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0xff8080+1000*i,
						strtemp.c_str());
				}
				else if((i+1)%3==0)
				{
					barlayer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x80ff80+1000*i,
						strtemp.c_str());
				}
				else
				{
					barlayer->addDataSet(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), 0x8080ff+1000*i,
						strtemp.c_str());
				}
			}
			else
			{
				if((i+1)%2==0)
				{
					barlayer->addDataSet(DoubleArray(y, (int)(sizeof(y) / sizeof(y[0]))), 0xff8080+1000*i,
						strtemp.c_str());
				}
				else if((i+1)%3==0)
				{
					barlayer->addDataSet(DoubleArray(y, (int)(sizeof(y) / sizeof(y[0]))), 0x80ff80+1000*i,
						strtemp.c_str());
				}
				else
				{
					barlayer->addDataSet(DoubleArray(y, (int)(sizeof(y) / sizeof(y[0]))), 0x8080ff+1000*i,
						strtemp.c_str()); 
				}
			}

			barlayer->setBarGap(0.2, Chart::TouchBar);

		}
		else
		{
			LineLayer *layer = chart->addLineLayer();
			//std::string strtemp = satelliteNo[i].toStdString();
			
			//QByteArray arr = satelliteNo[i].toLocal8Bit(); //注意这一行
			//char * ch = arr.data();
			std::string strtemp = QTextCodec::codecForName("UTF-8")->fromUnicode(satelliteNo[i]).constData();

			double data[1];
			if(xcount==0)
			{
				data[0]=y[1];
				if((i+1)%2==0)
				{
					layer->addDataSet(DoubleArray(data,  (int)(sizeof(data) / sizeof(data[0]))), 0xff8080+1000*i,
					strtemp.c_str())->setDataSymbol(Chart::CircleSymbol+i, 9);
				}
				else if((i+1)%3==0)
				{
					layer->addDataSet(DoubleArray(data,  (int)(sizeof(data) / sizeof(data[0]))), 0x80ff80+1000*i,
					strtemp.c_str())->setDataSymbol(Chart::CircleSymbol+i, 9);
				}
				else
				{
					layer->addDataSet(DoubleArray(data,  (int)(sizeof(data) / sizeof(data[0]))), 0x8080ff+1000*i,
						strtemp.c_str())->setDataSymbol(Chart::CircleSymbol+i, 9);
				}
			}
			else
			{
				if((i+1)%2==0)
				{
					layer->addDataSet(DoubleArray(y,  (int)(sizeof(y) / sizeof(y[0]))), 0xff8080+1000*i,
						strtemp.c_str())->setDataSymbol(Chart::CircleSymbol+i, 9);
				}
				else if((i+1)%3==0)
				{
					layer->addDataSet(DoubleArray(y,  (int)(sizeof(y) / sizeof(y[0]))), 0x80ff80+1000*i,
						strtemp.c_str())->setDataSymbol(Chart::CircleSymbol+i, 9);
				}
				else
				{
					layer->addDataSet(DoubleArray(y,  (int)(sizeof(y) / sizeof(y[0]))), 0x8080ff+1000*i,
						strtemp.c_str())->setDataSymbol(Chart::CircleSymbol+i, 9);
				}
			}
			// Set the line width to 3 pixels
			layer->setLineWidth(1);
		}

	}

	if (IsBarchart==true)
	{
		if(xcount==0)
		{
			const char *labels[] = {"1"};
			chart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
		}
		else
		{
		chart->xAxis()->setDateScale(1,xcount+1);
		}
	}
	else
	{
		if(xcount==0)
		{
			const char *labels[] = {"1"};
			chart->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));
		}
		else
		{
			chart->xAxis()->setDateScale(0,xcount+1);
		}
	
	}
	// Add a title to the y-axis
	chart->yAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("数量").constData(), "simsun.ttc",10);
	chart->yAxis()->setAutoScale();

	chart->xAxis()->setAutoScale();
	if (index==0)
	{
		chart->xAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("时间/月").constData(), "simsun.ttc", 10);//"Time/week"
	}
	else if (index==1)
	{
		chart->xAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("时间/年").constData(), "simsun.ttc", 10);//"Time/week"
	}
	else if (index==2)
	{
		chart->xAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("时间/时").constData(), "simsun.ttc", 10);//"Time/week"
	}
	else if (index==3)
	{
		chart->xAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("时间/分").constData(), "simsun.ttc", 10);//"Time/week"
	}
	else if (index==4)
	{
		chart->xAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("时间/秒").constData(), "simsun.ttc", 8);//"Time/week"
	}
	else if (index==5)
	{
		chart->xAxis()->setTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("时间/天").constData(), "simsun.ttc", 8);//"Time/week"
	}
	// Output the chart
	//chart->makeChart("multishapebar.png");


	// Output the chart

	viewer->setChart(chart);

	//// Include tool tip for the chart
	//viewer->setImageMap(
	//	chart->getHTMLImageMap("", "", "title='{xLabel}: {value}'"));
	//delete chart;

}
void curve::on_pushButton_clicked()
{
	ui.tableWidget->clear();
	QStringList header;  
	header<<QString::fromLocal8Bit("卫星")<<QString::fromLocal8Bit("时间")<<ui.comboBox_3->currentText()<<QString::fromLocal8Bit("占总数百分比")<<QString::fromLocal8Bit("区域范围"); 
	ui.tableWidget->setColumnCount(5);   
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	if (ui.gridLayout->itemAt(0)!=NULL)
	{
		ui.gridLayout->removeWidget(viewer);
		viewer->clear();
		plot1->deleteLater();
	}
	plot1 = new FCustomPlot;
	viewer = new QChartViewer(this);

	int index = ui.linetypeBox->currentIndex();
	if (index==0)
	{
		IsBarchart=true;
	}
	else
	{
		IsBarchart=false;
	}
	IsMultisat=true;
	satlist.clear();
	satlist.append(ui.comboBox->currentText());
	sqllist.clear();
	QString startDate=ui.startdateEdit->text();//ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString endDate=ui.enddateEdit->text();//ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  


	startdatetime =QDateTime::fromString(startDate, "yyyy-MM-dd hh:mm:ss");  // ui.dateTimeEdit->dateTime();
	enddatetime = QDateTime::fromString(endDate, "yyyy-MM-dd hh:mm:ss"); //ui.dateTimeEdit_2->dateTime();
	//时间处理
	startYear=startdatetime.date().year();
	startMonth=startdatetime.date().month();
	startDay=startdatetime.date().day();

	endYear = enddatetime.date().year();
	endMonth=enddatetime.date().month();
	endDay = enddatetime.date().day();

	deltasecs = startdatetime.secsTo(enddatetime);
	deltadate=startdatetime.daysTo(enddatetime);;//option->delta_day(startYear,startMonth,startDay,endYear,endMonth,endDay);


	satlist.clear();
	paralist.clear();
	QString strSat = ui.comboBox->currentText();
	QString strsql;
	QString strpara = ui.comboBox_3->currentText();
	paralist.append(strpara);
	satlist.append(strSat);
	QString strlongitude = QString::fromLocal8Bit("经度");
	QString strlatitude = QString::fromLocal8Bit("纬度");
	if (sql.dboracle==true)
	{
		strsql = QString("select  %1 , %2 ,  %3 ,DATETIME  from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'").arg(strpara).arg(strlongitude).arg(strlatitude);
	}
	else
	{
		strsql =  QString("select  %1  , %2 ,  %3,DATETIME   from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# ").arg(strpara).arg(strlongitude).arg(strlatitude);
	}
	sqllist.append(strsql);
	strzone = QString::fromLocal8Bit("全球");
	this->timeID = ui.comboBox_2->currentIndex();
	setupChart();
	// Show the viewer
	viewer->show();
	ui.gridLayout->addWidget(viewer);

}
void curve::on_outButton_clicked()
{
	MyCutImageWidget *m_cutimage;
	m_cutimage = new MyCutImageWidget(this);
	m_cutimage->setWindowOpacity(0.2);
	m_cutimage->showFullScreen();
}
void curve::on_outputButton_clicked()
{
	QAbstractItemModel* model=ui.tableWidget->model();
	if (model->rowCount()==0||model==NULL)
	{
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("无数据");
		QMessageBox::information(this,str,str2);
		return;
	}

	QFileDialog dlg;
	dlg.setAcceptMode(QFileDialog::AcceptSave);

	dlg.setDirectory(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
	dlg.setNameFilter("*.xls");
	dlg.selectFile(QDate::currentDate().toString("yyyy-MM-dd.xls"));
	if(dlg.exec()!= QDialog::Accepted)
		return;
	QString filePath=dlg.selectedFiles()[0];
	if(OdbcExcel::saveFromTable(0,filePath,ui.tableWidget,"")) {
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("保存成功");
		QMessageBox::information(this,str,str2);
	}
	else{
		QString str = str.fromLocal8Bit("错误");
		QString msg=str.fromLocal8Bit("保存失败！\n\r")+OdbcExcel::getError();
		QMessageBox::critical(this,str,msg);
	}
}
void curve::siftPositionCheck()
{
	/*if(IssiftID==true) return;
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
	}*/
}
void curve::siftIDCheck()
{
	//if(IssiftPosition==true) return;
	//if (ui.checkBox_2->checkState()==Qt::Checked)
	//{
	//	ui.checkBox->setEnabled(false);
	//	ui.longitudeIDEdit->setEnabled(true);
	//	ui.latitudeIDEdit->setEnabled(true);
	//	IssiftID=true;
	//}
	//else
	//{
	//	ui.checkBox->setEnabled(true);
	//	ui.longitudeIDEdit->setEnabled(false);
	//	ui.latitudeIDEdit->setEnabled(false);
	//	IssiftID=false;
	//}
}
void curve::onChanged(int index)
{
	if (ui.gridLayout->itemAt(0)!=NULL)
	{
		ui.gridLayout->removeWidget(viewer);
		viewer->clear();
	}
	viewer = new QChartViewer(this);

	index = ui.linetypeBox->currentIndex();
	if (index==0)
	{
		IsBarchart=true;
	}
	else
	{
		IsBarchart=false;
	}
	setupChart();
	viewer->show();
	ui.gridLayout->addWidget(viewer);
}
void curve::Changed()
{
	this->ui.listWidget->clear();
	QString strSat = ui.comboBox->currentText();
	QStringList chipList;
	QSqlQuery query(*sql.db);
	query.exec("select*from SATELLITE_PUBLIC where SATELLITENO = '"+strSat+"' order by ID");
	while(query.next())
	{
		chipList.append(query.value(3).toString());
	}
	int count =chipList.count();
	QStringList list;
	QString strsql;
	if(count==1)
	{
		strsql= "select * from PARAMETER where CHIPNAME = '"+chipList.at(0)+"' order by ID";
	}
	if (count>1)
	{
		strsql= "select * from PARAMETER where ";
		for (int i=0;i<count;i++)
		{
			strsql +="CHIPNAME = '"+chipList.at(i)+"'";
			if(i!=count-1)
				strsql +=" or ";
		}
	}
	query.exec(strsql);
	while(query.next())
	{
		QString str = str.fromLocal8Bit("芯片名称：");
		list.append(str+query.value(1).toString());
		str = str.fromLocal8Bit("  规格：");
		list.append(str+query.value(2).toString());
		str = str.fromLocal8Bit("  封装：");
		list.append(str+query.value(3).toString());
		str = str.fromLocal8Bit("  额定电压：");
		list.append(str+query.value(4).toString());
		str = str.fromLocal8Bit("  总剂量：");
		list.append(str+query.value(5).toString());
		str = str.fromLocal8Bit("  单粒子翻转：");
		list.append(str+query.value(6).toString());
		str = str.fromLocal8Bit("  单粒子锁定：");
		list.append(str+query.value(7).toString());
		str = str.fromLocal8Bit("  厂家：");
		list.append(str+query.value(8).toString());
		str = str.fromLocal8Bit("  备注：");
		list.append(str+query.value(9).toString());
	}
	this->ui.listWidget->addItems(list);
}
void curve::on_startdateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.startdateEdit->setText(dateDlg->strDate);;
	}
}
void curve::on_enddateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.enddateEdit->setText(dateDlg->strDate);;
	}
}
void curve::on_refreshButton_clicked()
{
	refresh();
}
void curve::on_advancedButton_clicked()
{
	option= new Adcancedoption;
	option->setWindowModality(Qt::WindowModal);
	connect(option,SIGNAL(getsql(QStringList)), this, SLOT(setsql(QStringList)));
	option->show();
}
void curve::setsql(QStringList strlist)
{
	sqllist.clear();
	satlist.clear();
	paralist.clear();

	this->timeID = option->timeID;
	for (int i=0;i<strlist.count();i++)
	{
		sqllist.append(strlist.at(i));
	}
	IsMultisat=option->Ismultisat;

	for (int i=0;i<option->satlist.count();i++)
	{
		satlist.append(option->satlist.at(i));
	}

	for (int i=0;i<option->paralist.count();i++)
	{
		paralist.append(option->paralist.at(i));
	}

	startdatetime = option->startdatetime;
	enddatetime = option->enddatetime;
	deltadate =option->deltadate;
	deltasecs =option->deltasecs;
	startYear = option->startYear;
	startMonth = option->startMonth;
	startDay = option->startDay;

	strzone = option->strzone;
	//startYear = ui.dateTimeEdit->date().year();
	//startMonth=ui.dateTimeEdit->date().month();
	//startDay = ui.dateTimeEdit->date().day();
	//endYear = ui.dateTimeEdit_2->date().year();
	//endMonth=ui.dateTimeEdit_2->date().month();
	//endDay = ui.dateTimeEdit_2->date().day();
	//deltadate=option->delta_day(startYear,startMonth,startDay,endYear,endMonth,endDay);
	QStringList header;  
	ui.tableWidget->clear();
	if (IsMultisat)
	{	
		header<<QString::fromLocal8Bit("卫星")<<QString::fromLocal8Bit("时间")<<option->paralist.at(0)<<QString::fromLocal8Bit("占总数百分比")<<QString::fromLocal8Bit("区域范围"); 
	}
	else
	{
		header<<QString::fromLocal8Bit("参数名称")<<QString::fromLocal8Bit("时间")<<QString::fromLocal8Bit("数量")<<QString::fromLocal8Bit("占总数百分比")<<QString::fromLocal8Bit("区域范围"); 
	}	
	ui.tableWidget->setColumnCount(5);   
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	if (ui.gridLayout->itemAt(0)!=NULL)
	{
		ui.gridLayout->removeWidget(viewer);
		viewer->clear();
		plot1->deleteLater();
	}
	plot1 = new FCustomPlot;
	viewer = new QChartViewer(this);

	int index = ui.linetypeBox->currentIndex();
	if (index==0)
	{
		IsBarchart=true;
	}
	else
	{
		IsBarchart=false;
	}
	setupChart();
	// Show the viewer
	viewer->show();
	ui.gridLayout->addWidget(viewer);
}
void curve::setCompleter(const QString &text) {
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


	QString strsql= "select * from SATELLITE_PUBLIC where SATELLITENO like '"+text+"'";
	query.exec(strsql);
	if (query.next())return;

	QStringList list;
	for(int i = 0;i < vecSat.size();++i){

		if (vecSat[i].indexOf(text) != -1)
		{

			list.append(vecSat[i]);
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
	namelistview->setGeometry(this->x()+170, this->y()+210, 50, 100);
	namelistview->resize(100,200);
	namelistview->setFixedWidth(ui.comboBox->width());
	namelistview->show();
	//	connect(ui.comboBox,SIGNAL(editTextChanged (const QString & )),this,SLOT(setCompleter(const QString & )));  
}
void curve::completeText(const QModelIndex &index) {
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
curve::~curve()
{

}