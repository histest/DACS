#ifndef MULTISATDLG_H
#define MULTISATDLG_H

#include <QDialog>
#include "ui_multisatdlg.h"

class MultiSatDlg : public QDialog
{
	Q_OBJECT

public:
	MultiSatDlg(QWidget *parent = 0);
	~MultiSatDlg();
	void initUI();
	QString satellites[16];
	int satellitecount;
private:
	Ui::MultiSatDlg* ui;
	
	private slots:
		void on_okButton_clicked();
		void on_cancelButton_clicked();
		void on_addButton_clicked();
		void on_delButton_clicked();
};

#endif // MULTISATDLG_H
