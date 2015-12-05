#ifndef SOURCEDATAMANAGEMENT_H
#define SOURCEDATAMANAGEMENT_H

#include <QWidget>
#include "ui_sourcedatamanagement.h"
#include "advancedsearch.h"
class SourceDataManagement : public QWidget
{
	Q_OBJECT

public:
	SourceDataManagement(QWidget *parent = 0);
	~SourceDataManagement();
	Advancedsearch*search;
private:
	Ui::SourceDataManagement ui;
	void initUI();
	private slots:
		void on_refreshButton_clicked();
		void on_clearButton_clicked();
		void on_advancedButton_clicked();
		void advancedpreview(QString);
};

#endif // SOURCEDATAMANAGEMENT_H
