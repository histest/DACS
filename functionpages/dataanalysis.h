#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include "QFramer/ftabwidget.h"
#include "animationgradientlabel.h"
#include "curve.h"
class DataAnalysis : public FTabWidget
{
	Q_OBJECT
public:
    explicit DataAnalysis(QWidget *parent = 0);
    void initUI();
    void CurveDisplay();
    void ThreeDemensionDisplay();
};

#endif // DATAANALYSIS_H
