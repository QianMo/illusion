#include "stdafx.h"
#include "VisionMainFrame.h"
#include "SelectOneDirDialog.h"

SelectOneDirDialog::SelectOneDirDialog(QWidget *parent)
    : QDialog(parent)
{

    setup_ui();
}

SelectOneDirDialog::~SelectOneDirDialog()
{

}

//!
void SelectOneDirDialog::setup_ui()
{
    QVariant var;
    VisionMainFrame::instance()->read_config("recently", "allinone", var);
    allinone_path_ = var.toString();
    //ȥ��������ť
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    QLabel *lb_tmp = NULL;
    resize(750, 10);
    setWindowTitle(QString::fromLocal8Bit("����ALL IN ONE·��"));
    QGridLayout *grid_layout = new QGridLayout(this);
    grid_layout->setSpacing(6);
    grid_layout->setContentsMargins(5, 5, 5, 5);

    QLabel *lbl_tmp = NULL;
    int line_use = 0;

    lbl_tmp = new QLabel(QString::fromLocal8Bit("����ALL IN ONEĿ¼������Ŀ¼���з���һ��������"));
    grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 30);

    ++line_use;
    lbl_tmp = new QLabel(QString::fromLocal8Bit("ALL IN ONE�ļ�Ŀ¼��"));
    grid_layout->addWidget(lbl_tmp, line_use, 0, 1, 1);
    le_allinone_dir_ = new QLineEdit();
    le_allinone_dir_->setText(allinone_path_);

    grid_layout->addWidget(le_allinone_dir_, line_use, 1, 1, 30);
    bt_allinone_dir_ = new QPushButton(QString::fromLocal8Bit("ѡ��Ŀ¼"));
    grid_layout->addWidget(bt_allinone_dir_, line_use, 31, 1, 1);
    connect(bt_allinone_dir_, &QPushButton::clicked, this, &SelectOneDirDialog::on_select_dir);

    ++line_use;
    QPushButton *ok_button = new QPushButton(QString::fromLocal8Bit("ȷ��"));
    grid_layout->addWidget(ok_button, line_use, 10, 1, 1);
    connect(ok_button, &QPushButton::clicked, this, &SelectOneDirDialog::on_ok_clicked);

    QPushButton *cancel_button = new QPushButton(QString::fromLocal8Bit("ȡ��"));
    grid_layout->addWidget(cancel_button, line_use, 18, 1, 1);
    connect(cancel_button, &QPushButton::clicked, this, &SelectOneDirDialog::on_cancel_clicked);
}

//
void SelectOneDirDialog::on_select_dir()
{
    QFileDialog file_dialog;

    file_dialog.setWindowTitle(QString::fromLocal8Bit("ѡ��ALL IN ONE��Ŀ¼"));
    file_dialog.setDirectory(".\\");
    file_dialog.setFilter(QDir::AllDirs);
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setOption(QFileDialog::ShowDirsOnly);
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (file_dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString selected_path = file_dialog.selectedFiles()[0];

    QObject *bt_obj = sender();

    if (bt_obj == bt_allinone_dir_)
    {
        le_allinone_dir_->setText(selected_path);
    }
    else
    {
        Q_ASSERT(false);
    }
}


void SelectOneDirDialog::on_ok_clicked()
{
    allinone_path_ = le_allinone_dir_->text();
    QFileInfo file_info(allinone_path_);
    if (!file_info.isDir())
    {
        QMessageBox::warning(this,
                             QString::fromLocal8Bit("����"),
                             QString::fromLocal8Bit("��ѡ��ALL IN ONEĿ¼,���������ALL IN ONEĿ¼����������Ŀ¼��")
                            );
        le_allinone_dir_->setFocus();
        return;
    }

    this->setResult(QDialog::Accepted);
    accept();
    this->close();
    VisionMainFrame::instance()->write_config("recently", "allinone", allinone_path_);
    return;
}

//!ȡ�����
void SelectOneDirDialog::on_cancel_clicked()
{
    this->setResult(QDialog::Rejected);
    rejected();
    this->close();
}

//�õ��û�ѡ���Ŀ¼
void SelectOneDirDialog::get_path_str(QString &allinone_path) const
{
    allinone_path = allinone_path_;
}

