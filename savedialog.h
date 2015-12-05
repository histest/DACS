#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QPushButton>
namespace Ui {
    class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = 0);
    ~SaveDialog();

	QPushButton *btn_ok;
	QPushButton *btn_quit;
	QPushButton *btn_save;
//public:
//    Ui::SaveDialog *ui;
};

#endif // SAVEDIALOG_H
