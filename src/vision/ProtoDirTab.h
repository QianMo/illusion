#ifndef PROTO_DIR_TAB_H
#define PROTO_DIR_TAB_H



class ProtoDirTab : public QSplitter
{
	Q_OBJECT

public:
	ProtoDirTab(QWidget *parent);
	~ProtoDirTab();

protected:

	///��װUI
	void setup_ui();


protected:



	///�������̵�����
	QWidget *testboard_contents_ = NULL;

	///�������̵Ĳ���
	QWidget *test_opearter_ = NULL;




};

#endif // PROTO_DIR_TAB_H

