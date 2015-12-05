

#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H

#include "QFramer/fcenterwindow.h"
#include "functionpages/uielement.h"
#include "functionpages/qssbuilder.h"
#include "functionpages/dataanalysis.h"
#include "functionpages/systemmanagement.h"
#include "curve.h"
#include <QVBoxLayout>
#include "help.h"

class CenterWindow : public FCenterWindow
{
    Q_OBJECT

public:
    UIElement* uiElements;
    QssBuilder* qssBuilder;
	
public:
    explicit CenterWindow(QWidget *parent = 0);
    void initUI();
    void addWiggleWiget();
signals:

public slots:
//    void cloudAntimation();
};

#endif // CENTERWINDOW_H
