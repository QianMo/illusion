#ifndef EXCEL_DIR_H
#define EXCEL_DIR_H



class ExcelDirTab : public QSplitter
{
    Q_OBJECT

public:
    ExcelDirTab(QWidget *parent);
    ~ExcelDirTab();

protected:

    ///��װUI
    void setup_ui();


protected:



    ///�������̵�����
    QWidget *testboard_contents_ = NULL;

    ///�������̵Ĳ���
    QWidget *test_opearter_ = NULL;




};

#endif // EXCEL_DIR_H
