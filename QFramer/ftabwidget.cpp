

#include "ftabwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QStackedLayout>
FTabWidget::FTabWidget(QWidget *parent) :
    QWidget(parent)
{
    initUI();
    initConnect();
}

void FTabWidget::initUI()
{
    tabTile = new QWidget;
    tabTile->setObjectName(QString("FTabTile"));

    stackLayout = new QStackedLayout;
    tabLayout = new QVBoxLayout;
    tabLayout->addStretch();
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->setSpacing(50);
    tabTile->setLayout(tabLayout);

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tabTile);
    mainLayout->addLayout(stackLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

void FTabWidget::initConnect()
{


}

void FTabWidget::addWidget(const QString &tile, const QString &objectName, QWidget *widget,int index)
{
    buttonTitles  << tile;
    FCheckabelButton* button = new FCheckabelButton(tile);
    buttons.append(button);
//    button->setObjectName(objectName);
    button->setFixedSize(120, 70);
	button->setAutoFillBackground(true);
	if (index==1)
	{
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.office.excel.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
		
	}
	if (index==2)
	{
	
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.paper.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	if (index==3)
	{

		button->setStyleSheet("QPushButton{background-image:  url(:/skin/icons/dark/appbar.pin.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	if (index==4)
	{
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.medical.pulse.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	if (index==5)
	{
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.video.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	if (index==6)
	{
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.user.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	if (index==7)
	{
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.database.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	if (index==8)
	{
		button->setStyleSheet("QPushButton{background-image: url(:/skin/icons/dark/appbar.sort.alphabetical.ascending.png);background-repeat:no-repeat;background-position: center bottom;background-color: transparent;color: white;text-align: center bottom;padding-bottom: 2px;}"
			"QPushButton:hover{background-color: rgb(27, 118, 150);color: white;}"
			"QPushButton:pressed{background-color: rgb(70,130,180);color: white;}"
			"QPushButton:checked{background-color: rgb(34, 125, 155);color: white;}");
	}
	QFont f("ZYSong18030", 10);
	button->setFont(f);
    tabLayout->insertWidget(tabLayout->count() - 1, button);
    connect(button, SIGNAL(clicked()), this, SLOT(setButtonChecked()));
//    widget->setObjectName(objectName);
    stackLayout->addWidget(widget);
}

QList<FCheckabelButton*> FTabWidget::getButtons()
{
    return buttons;
}

void FTabWidget::setButtonChecked()
{
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons.at(i) == sender())
        {
            buttons.at(i)->setChecked(true);
            stackLayout->setCurrentIndex(i);
            emit indexChanged(i);
        }
        else{
            buttons.at(i)->setChecked(false);
        }
    }
}
