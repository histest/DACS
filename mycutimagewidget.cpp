//mycutimagewidget.cpp
#include "mycutimagewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QClipboard>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
//-----------------------------------------------------------------------
//
MyCutImageWidget::MyCutImageWidget(QWidget *parent) :
    QDialog(parent), m_IsLeftButton(false), m_IsSelected(false)
{
    saveDialog = new SaveDialog;
    saveDialog->hide();
    connect(saveDialog->btn_save,SIGNAL(clicked()),this,SLOT(Bsave()));
//    connect(saveDialog->btn_ok,SIGNAL(clicked()),this,SLOT(Bok()));
    connect(saveDialog->btn_quit,SIGNAL(clicked()),this,SLOT(Bquit()));
}

void MyCutImageWidget::Bquit()
{
    this->close();
	saveDialog->close();
}

void MyCutImageWidget::Bok()
{
    QPixmap tmpPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
                                            m_Rect.x(), m_Rect.y(), m_Rect.width(), m_Rect.height());

    QClipboard *tmpClipboard = QApplication::clipboard();
    tmpClipboard->setPixmap(tmpPixmap);
    emit sendParentShow();
    this->hide();
    saveDialog->hide();
}

void MyCutImageWidget::Bsave()
{
    if (m_IsSelected)
    {
//        QPoint tmpPoint = event->pos();
//        if (tmpPoint.x() > m_Rect.x() &&
//            tmpPoint.x() < (m_Rect.x() + m_Rect.width()) &&
//            tmpPoint.y() > m_Rect.y() &&
//            tmpPoint.y() < (m_Rect.y() + m_Rect.height()))
        {
            this->hide();
            saveDialog->hide();
            QPixmap tmpPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
                                                    m_Rect.x(), m_Rect.y(), m_Rect.width(), m_Rect.height());

            QClipboard *tmpClipboard = QApplication::clipboard();
            tmpClipboard->setPixmap(tmpPixmap);
            emit sendParentShow();
			QString str = str.fromLocal8Bit("保存");
            QString fileName = QFileDialog::getSaveFileName(this,
                    str,
                    "",
                    tr("Config Files (*.png)"));

                if (!fileName.isNull())
                {
                    //fileName是文件名
                    QFile file(fileName);
                    QPixmap iim = tmpPixmap;

                    if (!file.open(QIODevice::ReadWrite))
                    {
                           return;
                    }
                   QByteArray ba;
                   QBuffer buffer(&ba);
                   buffer.open(QIODevice::WriteOnly);
                   iim.save(&buffer,"png");
                   file.write(ba);
                   file.close();
				   saveDialog->close();
				   this->close();
				   QString str = str.fromLocal8Bit("提示");
				   QString str2 = str.fromLocal8Bit("保存成功！");
				   QMessageBox::information(this,str,str2);
                }
                else
                {
                    //点的是取消
                }
        }
    }
	
}

//-----------------------------------------------------------------------
//
void MyCutImageWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (m_IsSelected)
    {
        QPoint tmpPoint = event->pos();
        if (tmpPoint.x() > m_Rect.x() &&
            tmpPoint.x() < (m_Rect.x() + m_Rect.width()) &&
            tmpPoint.y() > m_Rect.y() &&
            tmpPoint.y() < (m_Rect.y() + m_Rect.height()))
        {
            this->hide();
            QPixmap tmpPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
                                                    m_Rect.x(), m_Rect.y(), m_Rect.width(), m_Rect.height());

            QClipboard *tmpClipboard = QApplication::clipboard();
            tmpClipboard->setPixmap(tmpPixmap);
            emit sendParentShow();
        }
    }
}

//-----------------------------------------------------------------------
//
void MyCutImageWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (m_IsLeftButton  && !m_IsSelected)
    {
        operPoint(event);
    }
}

//-----------------------------------------------------------------------
//
void MyCutImageWidget::mousePressEvent ( QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && !m_IsSelected)
    {
        m_IsLeftButton = true;
        m_OldPoint = event->pos();
    }
}

//-----------------------------------------------------------------------
//
void MyCutImageWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::RightButton && m_IsSelected)
    {
        m_IsSelected = false;

        m_Rect.setX(0);
        m_Rect.setY(0);
        m_Rect.setWidth(0);
        m_Rect.setHeight(0);

        update();

    }else if (event->button() == Qt::RightButton && !m_IsSelected)
        this->hide();

    if (event->button() == Qt::LeftButton  && !m_IsSelected)
    {
        m_IsSelected = true;
        operPoint(event);
    }

    int sx = m_Rect.x()+m_Rect.width()- 454;
    int sy = m_Rect.y()+m_Rect.height();
    saveDialog->setGeometry(sx,sy,saveDialog->width(),saveDialog->height());
    saveDialog->show();
}

//-----------------------------------------------------------------------
//
void MyCutImageWidget::paintEvent(QPaintEvent * event)
{
  //  QPainter p(this);
  //  p.fillRect(rect(), QColor(0,0xff,0,30));

    QPainter p1(this);
  //  p1.fillRect(m_Rect, QColor(255,255,255,255));
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);

    p1.setPen(pen);
    p1.drawRect(m_Rect);
}

//-----------------------------------------------------------------------
//
void MyCutImageWidget::operPoint(QMouseEvent * event)
{
    // 获取两个坐标点并进行创建矩形
    QPoint tmpPoint = event->pos();

    int x,y,w,h;
    if (m_OldPoint.x() < tmpPoint.x())
    {
        x = m_OldPoint.x();
        w = tmpPoint.x() - m_OldPoint.x();
    }
    else
    {
        x = tmpPoint.x();
        w = m_OldPoint.x() - tmpPoint.x();
    }

    //
    if (m_OldPoint.y() < tmpPoint.y())
    {
        y = m_OldPoint.y();
        h = tmpPoint.y() - m_OldPoint.y();
    }
    else
    {
        y = tmpPoint.y();
        h = m_OldPoint.y() - tmpPoint.y();
    }

    m_Rect.setX(x);
    m_Rect.setY(y);
    m_Rect.setWidth(w);
    m_Rect.setHeight(h);

    update();
}
