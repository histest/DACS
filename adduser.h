#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include "ui_adduser.h"
class ADDUSER : public QDialog
{
	Q_OBJECT

public:
	ADDUSER(QWidget *parent = 0);
	~ADDUSER();

private:
	Ui::ADDUSER ui;
	private slots:
		 void on_okButton_clicked();
		 void on_clearButton_clicked();
		 void initUI();
};

#endif // ADDUSER_H
