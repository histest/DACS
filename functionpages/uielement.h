#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <QProgressBar>
#include "QFramer/ftabwidget.h"
#include "animationgradientlabel.h"
#include "productmanagement.h"
#include "componentmanagement.h"
#include "sourcedatamanagement.h"
#include "dictionarymanagement.h"
class UIElement : public FTabWidget
{
    Q_OBJECT
private:
    QList<QProgressBar *> progressBars;
public:
    explicit UIElement(QWidget *parent = 0);
    void initUI();
    void ProductInfoManagement();
    void SourceInfoManagement();
    void ComponentInfoManagement();
	void DictionaryInfoManagement();
	ProductManagement* productmanagement;
	SourceDataManagement*sourcedatamanagement;
	ComponentManagement*componentmanagement;
	DictionaryManagement*dictionaryManagement;
};

#endif // UIELEMENT_H
