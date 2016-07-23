#include "stdafx.h"
#include "ExcelDirTab.h"

ExcelDirTab::ExcelDirTab(QWidget *parent)
    : QSplitter(parent)
{
    setup_ui();
}

ExcelDirTab::~ExcelDirTab()
{

}


void ExcelDirTab::setup_ui()
{
    //ע��Qt�Ļ��߷����󣬲��ֶ��󣬵����ٻ�����Ӷ���
    this->setContentsMargins(5, 5, 5, 5);
    this->setOrientation(Qt::Horizontal);

    //���õ����̹�������
    QScrollArea *boardshow_scrollarea = new QScrollArea(this);
    boardshow_scrollarea->setWidgetResizable(true);
    boardshow_scrollarea->resize(650, 650);
    testboard_contents_ = new QWidget();
    testboard_contents_->setGeometry(QRect(0, 0, 1000, 1000));
    boardshow_scrollarea->setWidget(testboard_contents_);
    boardshow_scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    boardshow_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->addWidget(boardshow_scrollarea);

    test_opearter_ = new QWidget();
    this->addWidget(test_opearter_);

    this->setStretchFactor(0, 5);
    this->setStretchFactor(1, 4);
}


