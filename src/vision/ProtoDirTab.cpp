#include "stdafx.h"
#include "ProtoDirTab.h"

ProtoDirTab::ProtoDirTab(QWidget *parent)
	: QSplitter(parent)
{
	setup_ui();
}

ProtoDirTab::~ProtoDirTab()
{

}


void ProtoDirTab::setup_ui()
{
	//ע��Qt�Ļ��߷����󣬲��ֶ��󣬵����ٻ�����Ӷ���
	this->setContentsMargins(5, 5, 5, 5);
	this->setOrientation(Qt::Horizontal);

	//���õ����̹�������

	proto_dir_tree_ = new QTreeWidget();
	proto_dir_tree_->setGeometry(QRect(0, 0, 1000, 600));

	this->addWidget(proto_dir_tree_);

	test_opearter_ = new QWidget();
	this->addWidget(test_opearter_);

	this->setStretchFactor(0, 4);
	this->setStretchFactor(1, 6);
}


