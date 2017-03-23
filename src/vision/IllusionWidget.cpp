#include "stdafx.h"
#include "OverrideCursor.h"
#include "VisionMainFrame.h"
#include "IllusionWidget.h"

IllusionWidget::IllusionWidget(QWidget *parent)
    : QSplitter(parent)
{
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
    proto_dir_tree_->setGeometry(QRect(0, 0, 400, 600));

    this->addWidget(proto_dir_tree_);

    show_illusion_tab_ = new QTabWidget();
    show_illusion_tab_->setTabPosition(QTabWidget::North);
    this->addWidget(show_illusion_tab_);

    this->setStretchFactor(0, 4);
    this->setStretchFactor(1, 10);

    proto_dir_tree_->setColumnCount(2);
    QStringList headers;
    headers << QString::fromLocal8Bit("��Ŀ�ֶ�")
            << QString::fromLocal8Bit("��ֵ");
    proto_dir_tree_->setHeaderLabels(headers);
    proto_dir_tree_->setColumnWidth(0, 300);
    proto_dir_tree_->setColumnWidth(1, 200);

    connect(proto_dir_tree_,
            &QTreeWidget::itemDoubleClicked,
            this,
            &IllusionWidget::item_double_clicked);

    proto_dir_tree_->setExpandsOnDoubleClick(false);

    show_illusion_tab_->setTabsClosable(true);
    //!
    show_readme_ = new QTextEdit(show_illusion_tab_);
    show_illusion_tab_->addTab(show_readme_,
                               QString::fromLocal8Bit("README�ļ�"));

    connect(show_illusion_tab_,
            &QTabWidget::tabCloseRequested,
            this,
            &IllusionWidget::close_tab);

}

void IllusionWidget::select_all()
{
    int root_count = proto_dir_tree_->topLevelItemCount();
    for (int i = 0; i < root_count; ++i)
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

void IllusionWidget::selected_item(QStringList &selected_message)
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
    proto_2_ils_map_ = Biko_Read_Config::instance()->get_proto_illusion_map();
    if (proto_2_ils_map_->size() <= 0)
    {
        return;
    }
    proto_dir_tree_->clear();
    QStringList root_list, child_list;
    //!https://www.iconfinder.com/icons/1106243/business_excel_table_icon#size=128
    for (auto iter = proto_2_ils_map_->begin();
         iter != proto_2_ils_map_->end();
         ++iter)
    {
        root_list.clear();
        root_list.append(QString::fromLocal8Bit("Proto �ļ���"));
        root_list.append(iter->first);
        QTreeWidgetItem *root = new QTreeWidgetItem(root_list, ITEM_PROTO_FILE);
        proto_dir_tree_->addTopLevelItem(root);
        root->setExpanded(true);
        root->setIcon(0, QIcon(".\\res\\icon\\illusion_proto.png"));
        for (size_t i = 0; i < iter->second.size(); ++i)
        {
            QTreeWidgetItem *father = NULL;
            child_list.clear();
            child_list.append(QString::fromLocal8Bit("Table Message ����:"));
            child_list.append(iter->second[i]->table_message_name_);
            father = new QTreeWidgetItem(child_list, ITEM_PROTO_TABLE_MESSAGE);
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
            father->setIcon(0, QIcon(".\\res\\icon\\illusion_table.png"));

            QTreeWidgetItem *child = NULL;
            child_list.clear();
            child_list.append(QString::fromLocal8Bit("Line Message ����:"));
            child_list.append(iter->second[i]->line_message_name_);
            child = new QTreeWidgetItem(child_list, ITEM_PROTO_LINE_MESSAGE);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_line.png"));
            child->setData(0, Qt::UserRole, QVariant::fromValue((void *)(iter->second[i])) );
            father->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("ע������:"));
            child_list.append(iter->second[i]->cfg_comment_name_);
            child = new QTreeWidgetItem(child_list, ITEM_PROTO_COMMENT_NAME);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_comment.png"));
            father->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("EXCEL�ļ�����:"));
            child_list.append(iter->second[i]->excel_file_name_);
            child = new QTreeWidgetItem(child_list, ITEM_EXCEL_FILE);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_excel.png"));
            father->addChild(child);
            QTreeWidgetItem *excel_file = child;

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
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_exist.png"));
            excel_file->addChild(child);

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
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_newer.png"));
            excel_file->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("EXCEL SHEET:"));
            child_list.append(iter->second[i]->excel_sheet_name_);
            child = new QTreeWidgetItem(child_list, ITEM_EXCEL_SHEET);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_sheet.png"));
            child->setData(0, Qt::UserRole, iter->second[i]->excel_file_name_);
            father->addChild(child);
            QTreeWidgetItem *excel_sheet = child;

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("�ֶ�����������չ��RPEATED��:"));
            child_list.append(QString::number(iter->second[i]->column_field_count_));
            child = new QTreeWidgetItem(child_list);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_number.png"));
            excel_sheet->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("�ֶα�����:"));
            child_list.append(QString::number(iter->second[i]->fullname_line_));
            child = new QTreeWidgetItem(child_list);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_title_line.png"));
            excel_sheet->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("FULLNAME��(��ѡ):"));
            child_list.append(QString::number(iter->second[i]->fieldsname_line_));
            child = new QTreeWidgetItem(child_list);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_fullname_line.png"));
            excel_sheet->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("��ȡ������ʼ��:"));
            child_list.append(QString::number(iter->second[i]->read_data_line_));
            child = new QTreeWidgetItem(child_list);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_readdata_line.png"));
            excel_sheet->addChild(child);

            child_list.clear();
            child_list.append(QString::fromLocal8Bit("����ļ�����:"));
            child_list.append(iter->second[i]->outer_file_name_);
            child = new QTreeWidgetItem(child_list, ITEM_OUTER_FILE);
            child->setIcon(0, QIcon(".\\res\\icon\\illusion_outer.png"));
            father->addChild(child);
        }
    }
}


//!
void IllusionWidget::item_double_clicked(QTreeWidgetItem *item, int colum)
{
    if (NULL == item)
    {
        return;
    }

    TREE_ITEM_TYPE item_type = static_cast<TREE_ITEM_TYPE>( item->type());
    switch (item_type)
    {
        case ITEM_PROTO_FILE:
        {
            QString proto_fname = item->text(1);
            show_proto_file(proto_fname);
            break;
        }
        case ITEM_PROTO_LINE_MESSAGE:
        {
            Illusion_Message *ils_msg = (Illusion_Message *)(item->data(0, Qt::UserRole).value<void *>());
            QString line_name = item->text(1);
            show_line_message(line_name, ils_msg);
            break;
        }
        case ITEM_EXCEL_SHEET:
        {
            QString excel_fname = item->data(0, Qt::UserRole).toString();
            QString sheet_name = item->text(1);
            show_excel_sheet(excel_fname, sheet_name);
            break;
        }
        case ITEM_OUTER_FILE:
        {
            QString outer_fname = item->text(1);
            show_outer_file(outer_fname);
            break;
        }
        default:
            break;
    }
}


void IllusionWidget::show_proto_file(const QString &file_name)
{
    QString proto_path = Biko_Read_Config::instance()->proto_path();
    QString proto_fname = proto_path + "\\" + file_name;
    QFile file(proto_fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(VisionMainFrame::instance(),
                              QString::fromLocal8Bit("����"),
                              QString::fromLocal8Bit("��PROTO�ļ�[%1]ʧ�ܡ�")
                              .arg(proto_fname));
        return;
    }
    QTextEdit *proto_text_edit = new QTextEdit(show_illusion_tab_);
    show_illusion_tab_->addTab(proto_text_edit,
                               QIcon(".\\res\\icon\\illusion_proto.png"),
                               file_name);
    proto_text_edit->setText(file.readAll());
    proto_text_edit->setReadOnly(true);
    show_illusion_tab_->setCurrentWidget(proto_text_edit);
}

//��ʾOuter�ļ�����Ȼ���ı���
void IllusionWidget::show_outer_file(const QString &file_name)
{
    QString outer_path = Biko_Read_Config::instance()->outer_path();
    QString outer_fname = outer_path + "\\" + file_name + ".txt";
    QFile file(outer_fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(VisionMainFrame::instance(),
                              QString::fromLocal8Bit("����"),
                              QString::fromLocal8Bit("��OUTER�ļ�[%1]ʧ�ܡ�")
                              .arg(outer_fname));
        return;
    }
    QTextEdit *outer_text_edit = new QTextEdit(show_illusion_tab_);
    show_illusion_tab_->addTab(outer_text_edit,
                               QIcon(".\\res\\icon\\illusion_outer.png"),
                               file_name);
    outer_text_edit->setText(file.readAll());
    outer_text_edit->setReadOnly(true);
    show_illusion_tab_->setCurrentWidget(outer_text_edit);
}

//!��ʾLine Message
void IllusionWidget::show_line_message(const QString &line_name,
                                       const Illusion_Message *ils_msg)
{
    QTableWidget *ils_msg_table = new QTableWidget();
    show_illusion_tab_->addTab(ils_msg_table,
                               QIcon(".\\res\\icon\\illusion_line.png"),
                               line_name);
    ils_msg_table->setColumnCount(5);
    ils_msg_table->verticalHeader()->setVisible(false);
    ils_msg_table->setRowCount(1);
    QStringList headers;
    headers << QString::fromLocal8Bit("���")
            << QString::fromLocal8Bit("�ֶ�����")
            << QString::fromLocal8Bit("�ֶ�fullname")
            << QString::fromLocal8Bit("�ֶ�����(type name)")
            << QString::fromLocal8Bit("�ֶα�ǩ(Label)");
    ils_msg_table->setColumnWidth(0, 50);
    ils_msg_table->setColumnWidth(1, 100);
    ils_msg_table->setColumnWidth(2, 400);
    ils_msg_table->setColumnWidth(3, 100);
    ils_msg_table->setColumnWidth(4, 100);
    ils_msg_table->setHorizontalHeaderLabels(headers);
    ils_msg_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ils_msg_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    size_t fields_count = ils_msg->column_fieldname_ary_.size();
    for (size_t i = 0; i < fields_count; ++i)
    {
        ils_msg_table->insertRow(i );
        ils_msg_table->setItem(i , 0, new QTableWidgetItem(QString::number(i)));
        ils_msg_table->setItem(i , 1, new QTableWidgetItem(ils_msg->column_fieldname_ary_[i]));
        ils_msg_table->setItem(i , 2, new QTableWidgetItem(ils_msg->column_fullname_ary_[i]));
        ils_msg_table->setItem(i , 3, new QTableWidgetItem(
                                   QString(ils_msg->line_field_desc_ary_[i]->type_name())));
        QString label_name;
        google::protobuf::FieldDescriptor::Label fields_label = ils_msg->line_field_desc_ary_[i]->label();
        if (fields_label == google::protobuf::FieldDescriptor::LABEL_REPEATED)
        {
            label_name = "REPATED";
        }
        else if (fields_label == google::protobuf::FieldDescriptor::LABEL_REQUIRED)
        {
            label_name = "REQUIRE";
        }
        else if (fields_label == google::protobuf::FieldDescriptor::LABEL_OPTIONAL)
        {
            label_name = "OPTIONAL";
        }
        else
        {
            Q_ASSERT(false);
        }
        ils_msg_table->setItem(i, 4, new QTableWidgetItem(label_name));

    }

    show_illusion_tab_->setCurrentWidget(ils_msg_table);
}


//!��ʾEXCEL��Sheet
void IllusionWidget::show_excel_sheet(const QString &file_name,
                                      const QString &sheet_name)
{
    OverrideWaitCursor wait;
    QString excel_path = Biko_Read_Config::instance()->excel_path();
    QString excel_fname = excel_path + "\\" + file_name;
    QtAxExcelEngine excel_engine;
    excel_engine.initialize(false);
    bool is_ok = excel_engine.open(excel_fname, false);
    if (false == is_ok)
    {
        return;
    }
    is_ok = excel_engine.loadSheet(sheet_name, true);
    if (false == is_ok)
    {
        return;
    }
    QTableWidget *sheet_table = new QTableWidget();
    show_illusion_tab_->addTab(sheet_table,
                               QIcon(".\\res\\icon\\illusion_sheet.png"),
                               sheet_name);
    excel_engine.readTableWidget(sheet_table);
    excel_engine.close();
    excel_engine.finalize();
    show_illusion_tab_->setCurrentWidget(sheet_table);
}

void IllusionWidget::close_tab(int index)
{
    QWidget *current_tab = show_illusion_tab_->widget(index);
    current_tab->close();
    show_illusion_tab_->removeTab(index);
    delete current_tab;
}

