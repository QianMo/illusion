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

	// SLOT
public:

public:

	//!ѡ�����е�Mesage
	void select_all();

	//!��ѡ���е�Message
	void select_none();

	//!ȡ�����е�ȡ�õ�ITEM
	void selected_item(QStringList &selected_message);

protected:

	///�������̵�����
	QTreeWidget *proto_dir_tree_ = NULL;

	///�������̵Ĳ���
	QWidget *test_opearter_ = NULL;


	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

