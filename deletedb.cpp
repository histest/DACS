#include "deletedb.h"
#include "connectsql.h"
extern ConnectSql sql;
Deletedb::Deletedb(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initUI();
	setStyleSheet("QPushButton{background-color: rgb(70,130,180);color: white;border-style: outset;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 0px;}"
		"QPushButton:pressed{background-color: rgb(50, 77, 92);color: white;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}"
		"QLineEdit{border-radius: 0px;}"
		"QTabWidget{ border-top: 0px solid }"
		"QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");
	ui.tableView->setStyleSheet("border: 1px solid gray;color: rgb(34, 125, 155);	background-color: white;selection-color: black;selection-background-color:rgb(34, 125, 155);");
	ui.tableView_2->setStyleSheet("border: 1px solid gray;color: rgb(34, 125, 155);	background-color: white;selection-color: black;selection-background-color:rgb(34, 125, 155);");
	ui.tableView_3->setStyleSheet("border: 1px solid gray;color: rgb(34, 125, 155);	background-color: white;selection-color: black;selection-background-color:rgb(34, 125, 155);");

}
void Deletedb::initUI()
{
	this->ui.comboBox->clear();
	this->ui.comboBox_2->clear();
	QSqlQuery query(*sql.db);
	query.exec("select*from PARAMETER order by ID");
	QStringList list,list2;
	bool Isexist=false;
	while(query.next())
	{
		list.append(query.value(1).toString());
	}
	query.exec("select*from SATELLITE_PUBLIC order by ID");
	QStringList satlist;
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
	this->ui.comboBox->addItems(satlist);
	//this->ui.comboBox_4->addItems(satlist);
	this->ui.comboBox_2->addItems(list);

	QStringList*namelist = new QStringList;
	namelist->append(QString::fromLocal8Bit("�ۼƸ�λ����"));
	namelist->append(QString::fromLocal8Bit("������λ����"));
	namelist->append(QString::fromLocal8Bit("��λԭ��"));	
	namelist->append(QString::fromLocal8Bit("�������"));	
	namelist->append(QString::fromLocal8Bit("���������IO��RAM��ROM��ַ"));	
	namelist->append(QString::fromLocal8Bit("��������"));	
	namelist->append(QString::fromLocal8Bit("�л���־"));	
	namelist->append(QString::fromLocal8Bit("�ӵ��־"));	
	namelist->append(QString::fromLocal8Bit("SRAM�����ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("PROM�����ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("˫���ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("SRAM˫���ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("PROM˫���ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("ROM״̬��־"));	
	namelist->append(QString::fromLocal8Bit("RAM״̬��־"));	
	namelist->append(QString::fromLocal8Bit("EEPROM״̬��־"));	
	namelist->append(QString::fromLocal8Bit("��������ĵ�ַ����"));	
	namelist->append(QString::fromLocal8Bit("EEPROM�����ַ"));	
	namelist->append(QString::fromLocal8Bit("EEPROM˫���ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("EEPROM�����ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("EEPROM�������"));	
	namelist->append(QString::fromLocal8Bit("MRAM˫���ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("MRAM�����ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("MRAM�������"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH˫���ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH�����ۼƴ���"));	
	namelist->append(QString::fromLocal8Bit("NOR_FLASH�������"));	
	namelist->append(QString::fromLocal8Bit("ͬ����������"));	

	ui.comboBox_3->clear();
	query.exec("select*from DICTIONARY order by ID");
	list.clear();
	while(query.next())
	{
		for (int i=0;i<list.count();i++)
		{
			if (query.value(1).toString()==list.at(i))
				Isexist=true;
		}
		if(!Isexist)
			list.append(query.value(1).toString());
		//vtpara.push_back(query.value(1).toString());
		Isexist=false;
	}
	ui.comboBox_3->addItems(list);
	ui.tableView_3->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

}
void Deletedb::on_previewButton_2_clicked()
{
	//QString startDate=ui.startdateEdit->text();  
	//QString endDate=ui.enddateEdit->text();  
	//QString str1=startDate;
	//QString str2=endDate;//+"%"
	model = new QSqlTableModel(this,*sql.db);
	model->setTable("SATELLITE"); 
	model->setEditStrategy(QSqlTableModel::OnManualSubmit); 
	QString strSatellite = ui.comboBox->currentText();
	model->setFilter(QObject::tr("SATELLITENO = '%1'").arg(strSatellite)); //������������ɸѡ
	model->select(); //��ʾ���
	// model->removeColumn(1); //����ʾname������,�����ʱ��Ӽ�¼��������Ե�ֵ��Ӳ��� 
	ui.tableView_2->setModel(model); 
//	ui.tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
void Deletedb::on_previewButton_3_clicked()
{
	QSqlQuery query(*sql.db);

	QString strChipName = ui.comboBox_2->currentText();
	//QString strSat = ui.comboBox_4->currentText();
	paramodel = new QSqlTableModel(this,*sql.db);

	paramodel->setEditStrategy(QSqlTableModel::OnManualSubmit); 
	paramodel->setTable("PARAMETER"); 
	paramodel->setFilter(QObject::tr("CHIPNAME = '%1'").arg(strChipName)); //������������ɸѡ
	//if (ui.radioButton->isChecked()==true)
	//{
	//	
	//}
	//else
	//{
	//	QString strsql;
	//	QStringList chipList;
	//	QSqlQuery query(*sql.db);
	//	query.exec("select*from SATELLITE_PUBLIC where SATELLITENO = '"+strSat+"' order by ID");
	//	while(query.next())
	//	{
	//		chipList.append(query.value(3).toString());
	//	}
	//	int count =chipList.count();
	//	if(count==1)
	//		strsql= " CHIPNAME = '"+chipList.at(0)+"'";
	//	if (count>1)
	//	{
	//		for (int i=0;i<count;i++)
	//		{
	//			strsql +="CHIPNAME = '"+chipList.at(i)+"'";
	//			//if(i!=count-1)
	//			//	strsql +=" or ";
	//		}
	//	}
	//	paramodel->setFilter(strsql); 
	//}
	paramodel->select(); //��ʾ���
	// model->removeColumn(1); //����ʾname������,�����ʱ��Ӽ�¼��������Ե�ֵ��Ӳ��� 
	ui.tableView->setModel(paramodel); 
}
void Deletedb::on_deleteButton_clicked()
{
	QItemSelectionModel *selections = ui.tableView_2->selectionModel(); //���ص�ǰ��ѡ��ģʽ
	QModelIndexList selecteds = selections->selectedIndexes(); //��������ѡ����ģ����Ŀ�����б�
    if (model==NULL)
    {
		return;
    }
	foreach (QModelIndex index, selecteds)
	{
		int curRow = index.row(); //ɾ�����б�ѡ�е���
		model->removeRow(curRow);
	}
	int ok = QMessageBox::warning(this,QString::fromLocal8Bit("ɾ��ѡ�е���!"),QString::fromLocal8Bit("��ȷ��ɾ����ǰѡȡ�е�����?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::Yes)
	{
		model->submitAll(); //�ύ�������ݿ���ɾ������
	} else {
		model->revertAll(); //�����ɾ��������
	}
	on_previewButton_2_clicked();

}
void Deletedb::on_deleteallButton_clicked()
{
	QString strsat = ui.comboBox->currentText();
	QSqlQuery query(*sql.db);
	int ok = QMessageBox::warning(this,QString::fromLocal8Bit("ȫ��ɾ��!"),QString::fromLocal8Bit("��ȷ��ɾ������������������?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::Yes)
	{
		if(query.exec("delete  from SATELLITE where SATELLITENO = '"+strsat+"'")&&query.exec("delete  from SATELLITE_PUBLIC where SATELLITENO = '"+strsat+"'")&&query.exec("delete  from SOURCEDATA where SATELLITENO = '"+strsat+"'"))
			QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��ɾ����"), QMessageBox::Yes);
		this->ui.comboBox->clear();
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
			}
			Isexist=false;
		}
		this->ui.comboBox->addItems(list);

		QSqlTableModel* model = new QSqlTableModel(this,*sql.db);
		ui.tableView_2->setModel(model);
	}
	else
	{
		return;
	}
}
void Deletedb::on_saveButton_clicked()
{
	switch(QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("ȷ���޸ģ�"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes))
	{
	case QMessageBox::No:
		return;
		break;
	case QMessageBox::Yes:
		break;
	default:
		break;
	}
	model->database().transaction(); 
	if (model->submitAll()) 
	{
		model->database().commit(); 
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�޸ĳɹ���"), QMessageBox::No);
	} else {
		model->database().rollback(); 
		//QMessageBox::warning(this, tr("tableModel"),tr("���ݿ����: %1").arg(model->lastError().text()));
		model->revertAll(); 
	}

}
void Deletedb::on_saveButton_2_clicked()
{
	switch(QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("ȷ���޸ģ�"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes))
	{
	case QMessageBox::No:
		return;
		break;
	case QMessageBox::Yes:
		break;
	default:
		break;
	}
	paramodel->database().transaction(); 
	if (paramodel->submitAll()) 
	{
		paramodel->database().commit(); 
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�޸ĳɹ���"), QMessageBox::No);
	} else {
		paramodel->database().rollback(); 
		//QMessageBox::warning(this, tr("tableModel"),tr("���ݿ����: %1").arg(model->lastError().text()));
		paramodel->revertAll(); 
	}

}
void Deletedb::on_deleteButton_2_clicked()
{
	QItemSelectionModel *selections = ui.tableView->selectionModel(); //���ص�ǰ��ѡ��ģʽ
	QModelIndexList selecteds = selections->selectedIndexes(); //��������ѡ����ģ����Ŀ�����б�
	if (paramodel==NULL)
	{
		return;
	}
	foreach (QModelIndex index, selecteds)
	{
		int curRow = index.row(); //ɾ�����б�ѡ�е���
		paramodel->removeRow(curRow);
	}
	int ok = QMessageBox::warning(this,QString::fromLocal8Bit("ɾ��ѡ�е���!"),QString::fromLocal8Bit("��ȷ��ɾ����ǰѡȡ�е�����?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::Yes)
	{
		paramodel->submitAll(); //�ύ�������ݿ���ɾ������
	} else {
		paramodel->revertAll(); //�����ɾ��������
	}
	on_previewButton_3_clicked();
	initUI();
}
void Deletedb::on_startdateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		//ui.startdateEdit->setText(dateDlg->strDate);;
	}
}
void Deletedb::on_enddateButton_clicked()
{
	dateDlg =new datedlg;
	if(dateDlg->exec()==QDialog::Accepted)
	{
		//ui.enddateEdit->setText(dateDlg->strDate);;
	}
}
void Deletedb::on_previewButton_4_clicked()
{
	QSqlQuery query(*sql.db);
	QString strName = ui.comboBox_3->currentText();
	dictionarymodel = new QSqlTableModel(this,*sql.db);
	dictionarymodel->setTable("DICTIONARY"); 
	dictionarymodel->setEditStrategy(QSqlTableModel::OnManualSubmit); 
	QString strsql = QString("%1 = %2").arg(QString::fromLocal8Bit("����")).arg(strName);
	dictionarymodel->setFilter(QObject::tr("NAME = '%1'").arg(strName));//(strsql); //������������ɸѡ
	dictionarymodel->select(); //��ʾ���
	// model->removeColumn(1); //����ʾname������,�����ʱ��Ӽ�¼��������Ե�ֵ��Ӳ��� 

	ui.tableView_3->setModel(dictionarymodel); 
	//	ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

}
void Deletedb::on_saveButton_3_clicked()
{
	switch(QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("ȷ���޸ģ�"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes))
	{
	case QMessageBox::No:
		return;
		break;
	case QMessageBox::Yes:
		break;
	default:
		break;
	}
	dictionarymodel->database().transaction(); 
	if (dictionarymodel->submitAll()) 
	{
		dictionarymodel->database().commit(); 
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�޸ĳɹ���"), QMessageBox::No);
	} else {
		dictionarymodel->database().rollback(); 
		//QMessageBox::warning(this, tr("tableModel"),tr("���ݿ����: %1").arg(model->lastError().text()));
		dictionarymodel->revertAll(); 
	}

}
void Deletedb::on_deleteButton_3_clicked()
{
	QItemSelectionModel *selections = ui.tableView_3->selectionModel(); //���ص�ǰ��ѡ��ģʽ
	QModelIndexList selecteds = selections->selectedIndexes(); //��������ѡ����ģ����Ŀ�����б�
	if (paramodel==NULL)
	{
		return;
	}
	foreach (QModelIndex index, selecteds)
	{
		int curRow = index.row(); //ɾ�����б�ѡ�е���
		dictionarymodel->removeRow(curRow);
	}
	int ok = QMessageBox::warning(this,QString::fromLocal8Bit("ɾ��ѡ�е���!"),QString::fromLocal8Bit("��ȷ��ɾ����ǰѡȡ�е�����?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::Yes)
	{
		dictionarymodel->submitAll(); //�ύ�������ݿ���ɾ������
	} else {
		dictionarymodel->revertAll(); //�����ɾ��������
	}
	on_previewButton_4_clicked();
	initUI();
}
void Deletedb::on_radioButton_clicked()
{
	//if (ui.radioButton->isChecked())
	//{
	//	ui.comboBox_4->setEnabled(false);
	//	ui.comboBox_2->setEnabled(true);
	//}
}
void Deletedb::on_radioButton_2_clicked()
{
	//if (ui.radioButton_2->isChecked())
	//{
	//	ui.comboBox_2->setEnabled(false);
	//	ui.comboBox_4->setEnabled(true);
	//}
}
void Deletedb::on_deleteAllButton_clicked()
{
	QSqlQuery query(*sql.db);
	int ok = QMessageBox::warning(this,QString::fromLocal8Bit("ȫ��ɾ��!"),QString::fromLocal8Bit("��ȷ��ɾ������оƬ������?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::Yes)
	{
		QString strsql;
		if (sql.dboracle==true)
		{
			strsql ="delete  from PARAMETER "; 
		}
		else
		{
			strsql ="delete * from PARAMETER ";
		}
		if(query.exec(strsql))
			QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��ɾ����"), QMessageBox::Yes);
		this->ui.comboBox_2->clear();
		QSqlTableModel* model = new QSqlTableModel(this,*sql.db);
		ui.tableView->setModel(model);
	}
	else
	{
		return;
	}
}
void Deletedb::on_deleteallButton_2_clicked()
{
	QSqlQuery query(*sql.db);
	int ok = QMessageBox::warning(this,QString::fromLocal8Bit("ȫ��ɾ��!"),QString::fromLocal8Bit("��ȷ��ɾ�����������ֵ���?"),QMessageBox::Yes,QMessageBox::No);
	if(ok == QMessageBox::Yes)
	{
		QString strsql;
		
		if (sql.dboracle==true)
		{
			strsql ="delete  from DICTIONARY "; 
		}
		else
		{
			strsql ="delete * from DICTIONARY ";
		}
		if(query.exec(strsql))
			QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��ɾ����"), QMessageBox::Yes);
		this->ui.comboBox_3->clear();
		QSqlTableModel* model = new QSqlTableModel(this,*sql.db);
		ui.tableView_3->setModel(model);
	}
	else
	{
		return;
	}
}
Deletedb::~Deletedb()
{

}
