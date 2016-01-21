#ifndef COMPONENTMANAGEMENT_H
#define COMPONENTMANAGEMENT_H

#include <QWidget>
#include "ui_componentmanagement.h"
#include<QFileDialog>
#include "odbcexcel.h"
class ComponentManagement : public QWidget
{
	Q_OBJECT

public:
	ComponentManagement(QWidget *parent = 0);
	void DisableInput();
	Ui::ComponentManagement ui;
	~ComponentManagement();
signals:  
	void  refresh(QString);  
	
private:
	
	private slots:
		void on_openButton_clicked();
		void on_radioButton_clicked();
		void on_radioButton_2_clicked();
		void on_inputButton_clicked();
		void on_outButton_clicked();
		void on_queryButton_clicked();
		void on_previewButton_clicked();
		void on_previewButton_2_clicked();
		void initUI();

};

#endif // COMPONENTMANAGEMENT_H
