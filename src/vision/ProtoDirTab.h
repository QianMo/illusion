#ifndef PROTO_DIR_TAB_H
#define PROTO_DIR_TAB_H



class ProtoDirTab : public QSplitter
{
	Q_OBJECT

public:
	ProtoDirTab(QWidget *parent);
	~ProtoDirTab();

	//!
	void loead_illusion();

protected:

	///��װUI
	void setup_ui();


protected:



	///�������̵�����
	QTreeWidget *proto_dir_tree_ = NULL;

	///�������̵Ĳ���
	QWidget *test_opearter_ = NULL;


	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

