#ifndef SELECTEACHDIRDIALOG_H
#define SELECTEACHDIRDIALOG_H



class QLabel;
class QLineEdit;
class QPushButton;

class SelectEachDirDialog : public QDialog
{
    Q_OBJECT

public:

    //
    SelectEachDirDialog(QWidget *parent);
    ~SelectEachDirDialog();


private:

    //!���UI
    void setup_ui();

    //!SLOT
    //! ѡ��Ŀ¼
    void on_select_dir();

    //!ȷ�ϵ��
    void on_ok_clicked();

    //!ȡ�����
    void on_cancel_clicked();

public:

    //!�õ��û�ѡ���Ŀ¼
    void get_path_str(QString &proto_path,
                      QString &excel_path,
					  QString &outer_path,
					  QStringList &import_list) const;
private:

	//
	static const size_t MAX_IMPORT_DIR_SIZE = 5;

private:


    //!�ļ�Ŀ¼�����
	
    QLineEdit *le_proto_dir_ = NULL;
    QLineEdit *le_excel_dir_ = NULL;
	QLineEdit *le_outer_dir_ = NULL;
	QLineEdit *le_import_list_[MAX_IMPORT_DIR_SIZE];

    //!ѡ��Ŀ¼��Ŀ¼
    QPushButton *bt_proto_dir_ = NULL;
    QPushButton *bt_outer_dir_ = NULL;
	QPushButton *bt_excel_dir_ = NULL;
	QPushButton *bt_import_list_[MAX_IMPORT_DIR_SIZE];
	


    //!�ļ�Ŀ¼���ַ���
    QString proto_path_;
    QString excel_path_;
	QString outer_path_;

	//!import��path�б������ж��
	QStringList import_list_;

};

#endif // SELECTEACHDIRDIALOG_H
