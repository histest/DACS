//mycutimagewidget.h
#ifndef MYCUTIMAGEWIDGET_H
#define MYCUTIMAGEWIDGET_H

#include <QDialog>
#include <QPoint>
#include <QRect>
#include "savedialog.h"

class MyCutImageWidget : public QDialog
{
    Q_OBJECT
public:
    explicit MyCutImageWidget(QWidget *parent = 0);
    SaveDialog *saveDialog;

protected:
    virtual void mouseDoubleClickEvent (QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void paintEvent(QPaintEvent * event);

    void operPoint(QMouseEvent * event);
signals:
    void sendParentShow(void);

public slots:
    void Bok();
    void Bquit();
    void Bsave();
protected:
    QPoint m_OldPoint;
    bool m_IsLeftButton;
    bool m_IsSelected;

    QRect m_Rect;

};

#endif // MYCUTIMAGEWIDGET_H
