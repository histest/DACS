#include "osgdisplay.h"
#include <iostream>
#include "connectsql.h"
#include "multisatdlg.h"
#include "PickHandler.h"
#include"mycutimagewidget.h"
#include "mydefine.h"
#include <vector>  
using namespace std;
vector <QString> veSat; 
extern bool Isfresh;
extern ConnectSql sql;
CPickHandler* pickhandler;

osgdisplay::osgdisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(DisplayOSG()));
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
}
void osgdisplay::initUI()
{
	veSat.clear();
	this->ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());
		veSat.push_back(query.value(1).toString());
	}
	this->ui.comboBox->addItems(list);
	onChanged();
	satellitecount =0;
	osg::Group*	Root  = new osg::Group;
	viewWidget = new ViewerWidget(osgViewer::ViewerBase::SingleThreaded,Root);
	ui.gridLayout->addWidget(viewWidget);

	IssiftPosition=false;
	IssiftID=false;
	Isblank=true;
	QStringList header;  
	header<<QString::fromLocal8Bit("卫星")<<QString::fromLocal8Bit("区域编号")<<QString::fromLocal8Bit("单错次数")<<QString::fromLocal8Bit("占总数百分比")<<QString::fromLocal8Bit("区域中心经纬度"); 
	ui.tableWidget->setColumnCount(5);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	viewWidget->view->addEventHandler( new osgViewer::StatsHandler );
	viewWidget->view->setCameraManipulator( new osgGA::TrackballManipulator );
	pickhandler = new CPickHandler((viewWidget->updateText.get()));
	pickhandler->tubeWellNode=NULL;
	pickhandler->lastTubeWellNode=NULL;
	pickhandler->pickcount=0;
	viewWidget->view->getEventHandlers().push_front(pickhandler);//new CPickHandler(mViewer));
	viewWidget->view->setSceneData(InitSceneGraph());

	QStringList*namelist = new QStringList;
	namelist->append(QString::fromLocal8Bit("累计复位次数"));
	namelist->append(QString::fromLocal8Bit("连续复位次数"));
	namelist->append(QString::fromLocal8Bit("复位原因"));	
	namelist->append(QString::fromLocal8Bit("单错次数"));	
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
	ui.comboBox_3->addItems(*namelist);
}
void osgdisplay::refresh()
{
	this->ui.comboBox->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list;
	while(query.next())
	{
		list.append(query.value(1).toString());

	}
	this->ui.comboBox->addItems(list);
	IsStop=true;

}
void osgdisplay::comboBox_editTextChanged()
{
	refresh();
}
void osgdisplay::siftPositionCheck()
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
void osgdisplay::siftIDCheck()
{
	/*if(IssiftPosition==true) return;
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
	}*/
}
void osgdisplay::multiSatcheck()
{
	//this->ui.listWidget->clear();
	//if (ui.multiSatcheckBox->checkState()==Qt::Checked)
	//{
	//	ui.comboBox->setEnabled(false);
	//	MultiSatDlg multisatdlg;
	//	if(multisatdlg.exec()==QDialog::Accepted)
	//	{
	//		satellitecount = multisatdlg.satellitecount;			
	//		for (int i=0;i<satellitecount;i++)
	//		{
	//			QString strSat=multisatdlg.satellites[i];
	//			satelliteNo[i]=strSat;

	//			QSqlQuery query(*sql.db);
	//			query.exec("select*from PARAMETER where SATELLITENO = '"+strSat+"'order by ID");
	//			QStringList list;
	//			while(query.next())
	//			{

	//				QString str = str.fromLocal8Bit("-卫星编号：");
	//				QString strID = QString::number(i+1);
	//				str = strID+str;
	//				list.append(str+query.value(1).toString());	
	//				str = str.fromLocal8Bit("芯片名称：");
	//				list.append(str+query.value(3).toString());
	//				str = str.fromLocal8Bit("规格：");
	//				list.append(str+query.value(4).toString());
	//				str = str.fromLocal8Bit("封装：");
	//				list.append(str+query.value(5).toString());
	//				str = str.fromLocal8Bit("额定电压：");
	//				list.append(str+query.value(6).toString());
	//				str = str.fromLocal8Bit("额定电流：");
	//				list.append(str+query.value(7).toString());
	//				str = str.fromLocal8Bit("抗辐射指标：");
	//				list.append(str+query.value(8).toString());
	//			}
	//			this->ui.listWidget->addItems(list);
	//		}

	//	};

	//}
	//else
	//{
	//	//		ui.tableView->setVisible(true);
	//	ui.comboBox->setEnabled(true);
	//	onChanged();
	//}
}
void calculateposition(double longitude, double latitude,double  coordinate[2])
{
	coordinate[0]=(longitude+30)/15;
	coordinate[1]=(90-latitude)/10;
}
osg::Group*osgdisplay::InitSceneGraph(void)
{
	//double LongitudeCount = ui.longitudeBox->currentText().toDouble();
	//double LatitudeCount = ui.latitudeBox->currentText().toDouble();
	double UnitLongitude = (double)360/LongitudeCount;
	double UnitLatitude = (double)180/LatitudeCount;

	osg::Group*	mRoot  = new osg::Group;



	//地图
	osg::ref_ptr<osg::MatrixTransform> matrixEarth = new osg::MatrixTransform;
	osg::ref_ptr<osg::TessellationHints> pHints = new osg::TessellationHints;
	pHints->setDetailRatio(5.0f);
	osg::ref_ptr<osg::Sphere> sphereEarth = new osg::Sphere(osg::Vec3(0.0,0.0,0.0),osg::WGS_84_RADIUS_POLAR);
	osg::ref_ptr<osg::ShapeDrawable> pShapeOfEarth = new osg::ShapeDrawable(sphereEarth,pHints);
	pShapeOfEarth->setName("Earth");
	
	osg::Geode* pGeodeOfEarth = new osg::Geode;
	pGeodeOfEarth->addDrawable(pShapeOfEarth.get());
	std::string strFileName = osgDB::findDataFile(":/Earth1.jpg");//
	osg::ref_ptr<osg::Texture2D> textureEarth = new osg::Texture2D(osgDB::readImageFile(strFileName));
	pGeodeOfEarth->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureEarth);
	pGeodeOfEarth->setNodeMask(1);
	pGeodeOfEarth->setName("Earth");
	mRoot ->addChild (pGeodeOfEarth);

	if(Isblank==true)
	{
		osg::ref_ptr <osg::Geode> spheregeode = new osg::Geode ;
		osg::ref_ptr<osg::TessellationHints> pHints = new osg::TessellationHints;
		pHints->setDetailRatio(0.0001f);
		osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3(0.0,0.0,0.0),0.5);
		osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(sphere,pHints);
		spheregeode->addDrawable(shape.get()); 
		//mRoot->addChild(spheregeode);
		return mRoot;
	}


	//绘制统计信息
	osg::ref_ptr <osg::Vec3Array > Num2 = new osg::Vec3Array ; 
	osg::ref_ptr <osg::Vec3Array > allPoints2 = new osg::Vec3Array ; 

	double FixPosition[3]={0.0};
	CEarthAgent	clsEarthAgent;
	SGeographyElements HLB;
	double longitude=0.0;
	double latitude = 0.0;
	QSqlQuery query(*sql.db);
	QString strsql;

	int totalcount[256]={0};
	int rowcount=0;

	satellitecount = sqllist.count(); 
	////经纬度范围初始化
	//double startPointLongitude = ui.startLongitudeEdit->text().toDouble();
	//double startPointLatitude = ui.startLatitudeEdit->text().toDouble();

	//double endPointLongitude = ui.endLongitudeEdit->text().toDouble();
	//double endPointLatitude = ui.endLatitudeEdit->text().toDouble();

	//if (startPointLongitude<0)
	//{
	//	startPointLongitude+=360;
	//}
	//if (endPointLongitude<=0)
	//{
	//	endPointLongitude+=360;
	//}
	//if (startPointLongitude>endPointLongitude&&startPointLatitude<endPointLatitude)
	//{
	//	double temp=startPointLongitude;
	//	startPointLongitude=endPointLongitude;
	//	endPointLongitude=temp;
	//	temp=startPointLatitude;
	//	startPointLatitude=endPointLatitude;
	//	endPointLatitude=temp;
	//}
	//else if (startPointLongitude<endPointLongitude&&startPointLatitude<endPointLatitude)
	//{
	//	double temp=startPointLatitude;
	//	startPointLatitude=endPointLatitude;
	//	endPointLatitude=temp;
	//}
	//else if (startPointLongitude>endPointLongitude&&startPointLatitude>endPointLatitude)
	//{
	//	double temp=startPointLongitude;
	//	startPointLongitude=endPointLongitude;
	//	endPointLongitude=temp;
	//}

	////经纬度编号初始化
	//int longitudeID = ui.longitudeIDEdit->text().toInt();
	//int latitudeID= ui.latitudeIDEdit->text().toInt();
	//if(IssiftID==true)
	//{
	//	startPointLongitude=UnitLongitude*longitudeID;
	//	startPointLatitude=90-UnitLatitude*latitudeID;
	//	endPointLongitude=UnitLongitude*(longitudeID+1);
	//	endPointLatitude=90-UnitLatitude*(latitudeID+1);
	//}

	//时间处理
	//QStringList info3=ui.startdateEdit->text().split("-");
	//int startYear=info3[0].toInt();
	//int startMonth=info3[1].toInt();
	//int startDay=info3[2].toInt();
	//QString strstartdate,strstartYear,strstartMonth,strstartDay;
	//strstartYear=QString::number(startYear);
	//if (startMonth<10)
	//{
	//	strstartMonth="0"+QString::number(startMonth);
	//}
	//if (startDay<10)
	//{
	//	strstartDay="0"+QString::number(startDay);
	//}
	//strstartdate = strstartYear+strstartMonth+strstartDay;
	//int startdate =strstartdate.toInt();

	//QStringList info4=ui.enddateEdit->text().split("-");
	//int endYear=info4[0].toInt();
	//int endMonth=info4[1].toInt();
	//int endDay=info4[2].toInt();

	//QString strenddate,strendYear,strendMonth,strendDay;
	//strendYear=QString::number(endYear);
	//if (endMonth<10)
	//{
	//	strendMonth="0"+QString::number(endMonth);
	//}
	//else
	//{
	//	strendMonth=QString::number(endMonth);
	//}

	//if (endDay<10)
	//{
	//	strendDay="0"+QString::number(endDay);
	//}
	//else
	//{
	//	strendDay=QString::number(endDay);
	//}
	//strenddate = strendYear+strendMonth+strendDay;
	//int enddate =strenddate.toInt();
	//if(startdate>enddate) 
	//{
	//	QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("开始时间应早于结束时间"));
	//	return NULL;
	//}
	//计算各卫星在时间范围内总的单错数
	for (int k=0;k<satellitecount;k++)
	{
		query.exec(sqllist.at(k));
		while(query.next())
		{
			totalcount[k]+=query.value(0).toInt();
			rowcount++;
		}
	}
	int row=0;
	ui.progressBar->setRange(0,satellitecount*LongitudeCount*LatitudeCount);  
	ui.tableWidget->setRowCount(rowcount);
	for (int k=0;k<satellitecount;k++)
	{
		//QString strSat=satelliteNo[k];
		//if (sql.dboracle==true)
		//{
		//	strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"' order by ID";
		//}
		//else
		//{
		//	strsql = "select * from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# order by ID";//and
		//}
		//query.exec(strsql);
		/*strsql = "select * from SATELLITE where  SATELLITENO = '"+strSat+"'order by ID";
		query.exec(strsql);*/

		QString strSat,strPara;
		if (IsMultisat)
		{
			strSat	=satlist.at(k);
			satelliteNo[k] = strSat;
			strPara = paralist.at(0);
		}
		if (!IsMultisat)
		{
			strSat	=satlist.at(0);
		//	satelliteNo[k] = paralist.at(k);
			strPara = paralist.at(k);
		}
		query.exec(sqllist.at(k));
		QVector<double> x(2000), y(2000);
		double tempData[256][256]={0.0};
		double faultcount;
		while(query.next())
		{
			faultcount=query.value(0).toDouble();
			HLB.Longitude = query.value(1).toDouble();
			HLB.Latitude = query.value(2).toDouble();
			if(HLB.Longitude<0)
			{
				HLB.Longitude = HLB.Longitude+360;
			}
			int coordinateX,coordinateY;

			latitude = HLB.Latitude;
			longitude = HLB.Longitude;

			coordinateX=((int)longitude)/UnitLongitude;
			coordinateY=(90-(int)latitude)/UnitLatitude;
			//longitude=UnitLongitude/2+UnitLongitude*coordinateX;
			//latitude=90-UnitLatitude/2-UnitLatitude*coordinateY;
			//if (longitude>180)
			//{
			//	longitude=longitude-360;
			//}
			//HLB.Longitude =longitude;
			//HLB.Latitude =latitude;
			tempData[coordinateX][coordinateY]+=faultcount;
		}
		QString strsql2= "select * from PARAMETER where SATELLITENO = '"+strSat+"'order by ID";
		query.exec(strsql2);
		while(query.next())
		{
			HLB.Altitude = query.value(2).toDouble()*10+6378+k*5000;
		}
		HLB.Altitude *=1E2;


		for (int p=0;p<LongitudeCount;p++)
		{
			for (int q=0;q<LatitudeCount;q++)
			{
				faultcount =tempData[p][q];
				ui.progressBar->setValue(k*LatitudeCount*LongitudeCount+(p+1)*(q+1));  
				if (faultcount==0) continue;
				if (faultcount!=0)
				{

					double longitude=UnitLongitude/2+UnitLongitude*p;
					double latitude=90-UnitLatitude/2-UnitLatitude*q;

					//if (IssiftPosition==true)
					//{
					//	if (longitude>startPointLongitude&&longitude<endPointLongitude&&latitude<startPointLatitude&&latitude>endPointLatitude)
					//	{
					//	}
					//	else
					//	{
					//		continue;
					//	}
					//}
					if (longitude>180)
					{
						longitude=longitude-360;
					}
					ui.tableWidget->insertRow(row);
					ui.tableWidget->setItem(row,0,new QTableWidgetItem(strSat));
					ui.tableWidget->setItem(row,1,new QTableWidgetItem(QString::number(p)+","+QString::number(q))); 
					ui.tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(faultcount))); 
					ui.tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(faultcount*100/totalcount[k])+"%")); 
					ui.tableWidget->setItem(row,4,new QTableWidgetItem(QString::number(longitude)+","+QString::number(latitude))); 
					row++;


					HLB.Longitude =longitude-180;
					HLB.Latitude =latitude;

				//	HLB.Altitude *=1E2;
					HLB.Latitude *= ToRad;
					HLB.Longitude *=ToRad;
					clsEarthAgent.HLBToEarthFixedPosition(HLB,FixPosition);
				//	clsEarthAgent.RectangularElementToOrbitElement(FixPosition,Velocity,oA);



					osg::ref_ptr <osg::Geode> spheregeode = new osg::Geode ;
					osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
					hints->setDetailRatio(0.2f);
					osg::ref_ptr<osg::ShapeDrawable> shape; 

					shape = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(FixPosition[0],FixPosition[1],FixPosition[2]),fabs(faultcount)* 50000.0),hints);
					shape->setColor(osg::Vec4f (0.3*k+0.5, 1-0.2*k, 0.2, 1.0)) ;
					spheregeode->addDrawable(shape.get()); 


					string strtext; 
					stringstream ss;
					ss<<faultcount;//faultcount
					ss>>strtext; 

					string str = strPara.toStdString();//
					/*	QTextCodec *code = QTextCodec::codecForName("gb18030");
					if (code)    str= code->fromUnicode(strPara).data();*/


					QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

					QTextCodec* code = QTextCodec::codecForName("UTF-8");
					QString qstr = strPara+": "+QString::fromStdString(strtext);//QObject::tr("中国");
					str = code->fromUnicode(qstr).data();
					strtext=str;//+": "+strtext;//QString("%1").arg("单错次数").toStdString();//"FaultCount: =str+":"+strtext"strPara.toStdString()+":"+strtext
					
					
					string strLongitude,strLatitude;
					stringstream ssLongitude,ssLatitude;
					double temp;
					if(longitude<0) 
						temp=longitude*-1;
					else
						temp=longitude;
					ssLongitude<<temp;
					ssLongitude>>strLongitude; 
					strLongitude = "Longitude: "+ strLongitude;
					if(longitude<0)
						strLongitude+="W";
					else
						strLongitude+="E";
					
					if(latitude<0) 
						temp=latitude*-1;
					else
						temp=latitude;
					ssLatitude<<temp;
					ssLatitude>>strLatitude;
					strLatitude = "Latitude: "+strLatitude;
					if(latitude<0)
						strLatitude+="S";
					else
						strLatitude+="N";
					strtext="SatelliteNo: "+strSat.toStdString()+"   \n"+strLongitude+"   \n"+strLatitude+"   \n"+strtext;

					spheregeode->setName(strtext);

					mRoot->addChild(spheregeode); 
//					tempcount++;
				
				}
			}
		}
		

	}
//	ui.progressBar->setValue(rowcount);


	//地图
	//osg::ref_ptr<osg::MatrixTransform> matrixEarth = new osg::MatrixTransform;
	//osg::ref_ptr<osg::TessellationHints> pHints = new osg::TessellationHints;
	//pHints->setDetailRatio(1.0f);
	//osg::ref_ptr<osg::Sphere> sphereEarth = new osg::Sphere(osg::Vec3(0.0,0.0,0.0),osg::WGS_84_RADIUS_POLAR);
	//osg::ref_ptr<osg::ShapeDrawable> pShapeOfEarth = new osg::ShapeDrawable(sphereEarth,pHints);
	//pShapeOfEarth->setName("Earth");

	//osg::Geode* pGeodeOfEarth = new osg::Geode;
	//pGeodeOfEarth->addDrawable(pShapeOfEarth.get());
	//std::string strFileName = osgDB::findDataFile(":/Earth1.jpg");//
	//osg::ref_ptr<osg::Texture2D> textureEarth = new osg::Texture2D(osgDB::readImageFile(strFileName));
	//pGeodeOfEarth->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureEarth);
	//pGeodeOfEarth->setNodeMask(1);
	//pGeodeOfEarth->setName("Earth");
	//mRoot ->addChild (pGeodeOfEarth);

	mRoot->addChild(viewWidget->createHUD(viewWidget->updateText.get()));
	return mRoot;
}
static int Discount=0;
void osgdisplay::DisplayOSG()
{
	ui.tableWidget->clear();
	Isblank=false;
	QStringList header;  
	header<<QString::fromLocal8Bit("卫星")<<QString::fromLocal8Bit("区域编号")<<QString::fromLocal8Bit("单错次数")<<QString::fromLocal8Bit("占总数百分比")<<QString::fromLocal8Bit("区域中心经纬度"); 
	ui.tableWidget->setColumnCount(5);  
	ui.tableWidget->setHorizontalHeaderLabels(header);  
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	if (!IsMultisat)
	{
		
	}
	IsMultisat=true;
	LongitudeCount =16;
	LatitudeCount = 8;
	paraID = ui.comboBox_3->currentIndex()+9;
	satlist.clear();
	satlist.append(ui.comboBox->currentText());
	sqllist.clear();
	QString startDate=ui.startdateEdit->text();//ui.dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");  
	QString endDate=ui.enddateEdit->text();//ui.dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");  

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

	//if (sql.dboracle==true)
	//{
	//	strsql = QString("select  %1 ,LONGITUDE, LATITUDE from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'").arg(strpara);
	//}
	//else
	//{
	//	strsql =  QString("select  %1 ,LONGITUDE, LATITUDE  from SATELLITE where SATELLITENO = '"+strSat+"' and DATETIME between #"+startDate+"# and #"+endDate+"# ").arg(strpara);
	//}
	sqllist.append(strsql);
	//ui.gridLayout->removeWidget(viewWidget);
	viewWidget->view->addEventHandler( new osgViewer::StatsHandler );
	viewWidget->view->setCameraManipulator( new osgGA::TrackballManipulator );

	viewWidget->view->removeEventHandler(pickhandler);
	pickhandler = new CPickHandler((viewWidget->updateText.get()));
	pickhandler->tubeWellNode=NULL;
	pickhandler->lastTubeWellNode=NULL;
	pickhandler->pickcount=0;

	viewWidget->view->getEventHandlers().push_front(pickhandler);//new CPickHandler(mViewer));

	viewWidget->view->setSceneData(InitSceneGraph());
/*	ui.gridLayout->addWidget(viewWidget);*/
	Discount++;
}
void osgdisplay::onChanged()
{
	this->ui.listWidget->clear();
	QString strSat = ui.comboBox->currentText();
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER where SATELLITENO = '"+strSat+"'order by ID");
	QStringList list;
	while(query.next())
	{
		QString str = str.fromLocal8Bit("芯片名称：");
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
	this->ui.listWidget->addItems(list);
}
void osgdisplay::on_outButton_clicked()
{
	MyCutImageWidget *m_cutimage;
	m_cutimage = new MyCutImageWidget(this);
	m_cutimage->setWindowOpacity(0.2);
	m_cutimage->showFullScreen();
}
void osgdisplay::on_outputButton_clicked()
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
	if(OdbcExcel::saveFromTable(filePath,ui.tableWidget,"")) {
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
void osgdisplay::on_startdateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.startdateEdit->setText(dateDlg->strDate);;
	}
}
void osgdisplay::on_enddateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		ui.enddateEdit->setText(dateDlg->strDate);;
	}
}
void osgdisplay::on_refreshButton_clicked()
{
	DisplayOSG();
}
void osgdisplay::on_refreshButton_2_clicked()
{
	//IsStop=false;
	refresh();
	onChanged();
}
void osgdisplay::on_advancedButton_clicked()
{
	option= new Adcancedoption;
	option->setWindowModality(Qt::WindowModal);
	connect(option,SIGNAL(getsql(QStringList)), this, SLOT(setsql(QStringList)));
	option->show();
}
void osgdisplay::setsql(QStringList strlist)
{
	sqllist.clear();
	satlist.clear();
	paralist.clear();
	this->timeID = option->timeID;
	this->LatitudeCount=option->LatitudeCount;
	this->LongitudeCount=option->LongitudeCount;
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


	Isblank=false;
	viewWidget->view->addEventHandler( new osgViewer::StatsHandler );
	viewWidget->view->setCameraManipulator( new osgGA::TrackballManipulator );
	viewWidget->view->removeEventHandler(pickhandler);
	pickhandler = new CPickHandler((viewWidget->updateText.get()));
	pickhandler->tubeWellNode=NULL;
	pickhandler->lastTubeWellNode=NULL;
	pickhandler->pickcount=0;
	viewWidget->view->getEventHandlers().push_front(pickhandler);//new CPickHandler(mViewer));
	osg::Group* root = new osg::Group;
	root= InitSceneGraph();
	viewWidget->view->setSceneData(root);
}
void osgdisplay::setCompleter(const QString &text) {
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
	for(int i = 0;i < veSat.size();++i){

		if (veSat[i].indexOf(text) != -1)
		{

			list.append(veSat[i]);
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
void osgdisplay::completeText(const QModelIndex &index) {
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
osgdisplay::~osgdisplay()
{

}
