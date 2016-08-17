#ifndef PROTO_DIR_TAB_H
#define PROTO_DIR_TAB_H



class IllusionWidget : public QSplitter
{
	Q_OBJECT

public:
	IllusionWidget(QWidget *parent);
	~IllusionWidget();

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

	//!PROTO �ļ���Ϣ
	QTreeWidget *proto_dir_tree_ = NULL;

	//!�������̵Ĳ���
	QTabWidget *show_illusion_tab_ = NULL;

	//!
	QTextEdit *show_readme_ = NULL;
	////!
	//QTableWidget *show_excel_sheet_ = NULL;
	////!
	//QTableWidget *show_message_fields_ = NULL;
	////!
	//QTextEdit *show_outer_file_ = NULL;


	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

