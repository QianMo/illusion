#ifndef OUTER_DIR_TAB_H
#define OUTER_DIR_TAB_H



class OuterDirTab : public QSplitter
{
	Q_OBJECT

public:
	OuterDirTab(QWidget *parent);
	~OuterDirTab();

protected:

	///��װUI
	void setup_ui();


protected:



	///�������̵�����
	QWidget *testboard_contents_ = NULL;

	///�������̵Ĳ���
	QWidget *test_opearter_ = NULL;




};

#endif // OUTER_DIR_TAB_H
