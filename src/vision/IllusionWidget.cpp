#include "stdafx.h"
#include "IllusionWidget.h"

IllusionWidget::IllusionWidget(QWidget *parent)
	: QSplitter(parent)
{
	proto_2_ils_map_ = Biko_Read_Config::instance()->get_proto_illusion_map();
	setup_ui();
}

IllusionWidget::~IllusionWidget()
{

}


void IllusionWidget::setup_ui()
{
	//ע��Qt�Ļ��߷����󣬲��ֶ��󣬵����ٻ�����Ӷ���
	this->setContentsMargins(5, 5, 5, 5);
	this->setOrientation(Qt::Horizontal);

	//���õ����̹�������

	proto_dir_tree_ = new QTreeWidget();
	proto_dir_tree_->setGeometry(QRect(0, 0, 1000, 600));

	this->addWidget(proto_dir_tree_);

	show_illusion_tab_ = new QTabWidget();
	show_illusion_tab_->setTabPosition(QTabWidget::North);
	this->addWidget(show_illusion_tab_);

	this->setStretchFactor(0, 4);
	this->setStretchFactor(1, 6);

	proto_dir_tree_->setColumnCount(2);
	QStringList headers;
	headers << QString::fromLocal8Bit("��Ŀ�ֶ�")
		<< QString::fromLocal8Bit("��ֵ");
	proto_dir_tree_->setHeaderLabels(headers);
	proto_dir_tree_->setColumnWidth(0, 259);
	proto_dir_tree_->setColumnWidth(1, 200);

	show_illusion_tab_->setTabsClosable(true);
	//!
	show_readme_ = new QTextEdit(show_illusion_tab_);
	show_illusion_tab_->addTab(show_readme_,
							   QString::fromLocal8Bit("README�ļ�"));
	
	//show_excel_sheet_ = new QTableWidget(show_illusion_tab_);
	//show_illusion_tab_->addTab(show_excel_sheet_, 
	//						   QString::fromLocal8Bit("EXCEL Sheet��:"));
	//show_message_fields_ = new QTableWidget(show_illusion_tab_);
	//show_illusion_tab_->addTab(show_message_fields_,
	//						   QString::fromLocal8Bit("Message�ֶ�:"));
	//show_outer_file_ = new QTextEdit(show_illusion_tab_);
	//show_illusion_tab_->addTab(show_outer_file_,
	//						   QString::fromLocal8Bit("OUTER�ļ�:"));

}

void IllusionWidget::select_all()
{
	int root_count = proto_dir_tree_->topLevelItemCount();
	for (int i=0;i<root_count;++i)
	{
		QTreeWidgetItem *root = proto_dir_tree_->topLevelItem(i);
		QTreeWidgetItem *father = root->child(0);
		father->setCheckState(0, Qt::Checked);
	}
}

void IllusionWidget::select_none()
{
	int root_count = proto_dir_tree_->topLevelItemCount();
	for (int i = 0; i < root_count; ++i)
	{
		QTreeWidgetItem *root = proto_dir_tree_->topLevelItem(i);
		QTreeWidgetItem *father = root->child(0);
		father->setCheckState(0, Qt::Unchecked);
	}
}

void IllusionWidget::selected_item(QStringList & selected_message)
{
	int root_count = proto_dir_tree_->topLevelItemCount();
	for (int i = 0; i < root_count; ++i)
	{
		QTreeWidgetItem *root = proto_dir_tree_->topLevelItem(i);
		QTreeWidgetItem *father = root->child(0);
		if (father->checkState(0) == Qt::Checked)
		{
			selected_message.append(father->text(1));
		}
	}
}


//!
void IllusionWidget::loead_illusion()
{
	if (proto_2_ils_map_->size() <= 0)
	{
		return;
	}
	proto_dir_tree_->clear();
	QStringList root_list,child_list;
	//!
	for (auto iter = proto_2_ils_map_->begin(); 
		 iter != proto_2_ils_map_->end();
		 ++iter)
	{
		root_list.clear();
		root_list.append(QString::fromLocal8Bit("Proto �ļ���"));
		root_list.append(iter->first);
		QTreeWidgetItem *root = new QTreeWidgetItem(root_list);
		proto_dir_tree_->addTopLevelItem(root);
		root->setExpanded(true);

		for (size_t i = 0; i < iter->second.size(); ++i)
		{
			QTreeWidgetItem *father = NULL;
			child_list.clear();
			child_list.append(QString::fromLocal8Bit("Table Message ����:"));
			child_list.append(iter->second[i]->table_message_name_);
			father = new QTreeWidgetItem(child_list);
			root->addChild(father);
			father->setExpanded(true);
			father->setFlags(father->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
			if (iter->second[i]->excelcfg_is_newer_)
			{
				father->setCheckState(0, Qt::Checked);
			}
			else
			{
				father->setCheckState(0, Qt::Unchecked);
			}
			QTreeWidgetItem *child = NULL;
			child_list.clear();
			child_list.append(QString::fromLocal8Bit("Line Message ����:"));
			child_list.append(iter->second[i]->line_message_name_);
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("EXCEL�ļ�����:"));
			child_list.append(iter->second[i]->excel_file_name_);
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("EXCEL SHEET:"));
			child_list.append(iter->second[i]->excel_sheet_name_);
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("����ļ�����:"));
			child_list.append(iter->second[i]->outer_file_name_);
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("�ֶ�����������չ��RPEATED��:"));
			child_list.append(QString::number(iter->second[i]->column_field_count_));
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("��Ӧ��EXCEL�ļ��Ƿ����:"));
			if (iter->second[i]->exist_excel_file_)
			{
				child_list.append(QString::fromLocal8Bit("��"));
			}
			else
			{
				child_list.append(QString::fromLocal8Bit("��"));
			}
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);

			child_list.clear();
			child_list.append(QString::fromLocal8Bit("��Ӧ��EXCEL�ļ��Ƿ����:"));
			if (iter->second[i]->excelcfg_is_newer_)
			{
				child_list.append(QString::fromLocal8Bit("��"));
			}
			else
			{
				child_list.append(QString::fromLocal8Bit("��"));
			}
			child = new QTreeWidgetItem(child_list);
			father->addChild(child);
		}
	}
}





