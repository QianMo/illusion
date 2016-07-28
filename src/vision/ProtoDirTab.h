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
	QTreeWidget *proto_dir_tree_ = NULL;

	///�������̵Ĳ���
	QWidget *test_opearter_ = NULL;


	std::map <QString, ILLUSION_MESSAGE_ARRAY> *proto_2_illusion_map_ = NULL;

};

#endif // PROTO_DIR_TAB_H

