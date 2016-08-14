#ifndef SELECTONEDIRDIALOG_H
#define SELECTONEDIRDIALOG_H



class QLabel;
class QLineEdit;
class QPushButton;

class SelectOneDirDialog : public QDialog
{
    Q_OBJECT

public:

    //
	SelectOneDirDialog(QWidget *parent);
    ~SelectOneDirDialog();


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
    void get_path_str(QString &allinone_path) const;

private:


    //!�ļ�Ŀ¼�����
    QLineEdit *le_allinone_dir_ = NULL;
    
    //!ѡ��Ŀ¼��Ŀ¼
    QPushButton *bt_allinone_dir_ = NULL;

    //!�ļ�Ŀ¼���ַ���
    QString allinone_path_;

};

#endif // SELECTONEDIRDIALOG_H
