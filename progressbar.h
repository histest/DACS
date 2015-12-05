#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QDialog>
#include "ui_progressbar.h"

class ProgressBar : public QDialog
{
	Q_OBJECT

public:
	ProgressBar(QWidget *parent = 0);
	~ProgressBar();

public:
	Ui::ProgressBar ui;
};

#endif // PROGRESSBAR_H
