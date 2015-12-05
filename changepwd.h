#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QDialog>
#include "ui_changepwd.h"

class Changepwd : public QDialog
{
	Q_OBJECT

public:
	Changepwd(QWidget *parent = 0);
	~Changepwd();

private:
	Ui::Changepwd ui;
	void  initUI();
	private slots:
		void on_okButton_clicked();
        void on_clearButton_clicked();
};

#endif // CHANGEPWD_H
