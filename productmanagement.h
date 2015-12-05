#ifndef PRODUCTMANAGEMENT_H
#define PRODUCTMANAGEMENT_H

#include <QWidget>
#include "ui_productmanagement.h"
#include<QFileDialog>
class ProductManagement : public QWidget
{
	Q_OBJECT

public:
	ProductManagement(QWidget *parent = 0);
	~ProductManagement();
	void DisableInput();
public slots:
	void initUI(QString);
	void refresh();
private:
	Ui::ProductManagement ui;
	bool IsStop;
	private slots:
		void on_openButton_clicked();
		void on_inputButton_clicked();
		void on_outButton_clicked();
		void on_queryButton_clicked();
		void on_previewButton_clicked();
		void on_previewButton_2_clicked();
		void on_refreshButton_clicked();
		void on_advancedsearchButton_clicked();
		void advancedpreview(QString);
		
};

#endif // PRODUCTMANAGEMENT_H
