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
	//
	enum TREE_ITEM_TYPE
	{
		ITEM_PROTO_FILE = 1001,
		ITEM_PROTO_TABLE_MESSAGE = 1002,
		ITEM_PROTO_LINE_MESSAGE = 1003,
		ITEM_EXCEL_FILE = 2001,
		ITEM_EXCEL_SHEET = 2002,
		ITEM_OUTER_FILE = 3001,
	};
	
	//!��װUI
	void setup_ui();

	//!��ʾPROTO�ļ�
	void show_proto_file(const QString &file_name);

	//!��ʾOuter�ļ�����Ȼ���ı���
	void show_outer_file(const QString &file_name);

	// SLOT
public:

	//!
	void item_double_clicked(QTreeWidgetItem*, int);

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
	
	//!
	QSTRING_2_ILLUSIONARY_MAP *proto_2_ils_map_ = NULL;


};

#endif // PROTO_DIR_TAB_H

