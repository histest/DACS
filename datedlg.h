#ifndef DATEDLG_H
#define DATEDLG_H

#include <QDialog>
#include "ui_datedlg.h"

class datedlg : public QDialog
{
	Q_OBJECT

public:
	datedlg(QWidget *parent = 0);
	~datedlg();
	QString strDate;

private:
	Ui::datedlg ui;
	public slots:
	void on_btn_ok_clicked();
	void on_btn_quit_clicked();
};

#endif // DATEDLG_H
