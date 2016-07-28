#ifndef VISIONMAINWND_H
#define VISIONMAINWND_H

#include <QtWidgets/QMainWindow>

class ProtoDirTab;
class ExcelDirTab;
class OuterDirTab;
//
enum PZ_TIP_LEVEL
{
	PZ_DEBUG,
	PZ_INFO,
	PZ_WARNNING,
	PZ_ERROR,
};


class VisionMainFrame : public QMainWindow
{
	Q_OBJECT

public:
	VisionMainFrame(QWidget *parent = 0);
	~VisionMainFrame();


	//�����Ϣ
	void out_info(PZ_TIP_LEVEL level, const QString &out_info);


protected:

	//!����UI����
	void setup_ui();

	//!�˵�����
	void setup_action();

	void eachdir_process();
	

	void allinone_process();

	//!�����˳�
	void exit_appliaction();

	//!
	void select_tab(int tab_id);

public:

	//����ʵ��ָ��
	static void set_instance(VisionMainFrame *inst);
	//��ȡʵ��ָ��
	static VisionMainFrame *instance();

	//!��ȡ����
	static bool read_config(const QString &section,
							const QString &key,
							QVariant &data);

	//!д������
	static bool write_config(const QString &section,
							 const QString &key,
							 const QVariant &data);

	//ʵ��ָ��
	static VisionMainFrame *instace_;


protected:

	//�����Ϣ����
	static const size_t MAX_TIPS_INFO_NUMBER = 32;

private:

	//!Action
	QAction *act_allinone_ = NULL;
	QAction *act_each_ = NULL;
	QAction *act_exit_ = NULL;


	//!�ָ���
	QSplitter *main_splitter_ = NULL;
	//!�����ڵ�TAB
	QTabWidget *main_tab_widget_ = NULL;

	//!�澯����
	QSound *m_alarm = NULL;

	//!�˵���menu bar
	QMenuBar *menubar_ = NULL;
	//!���˵�
	QMenu *mainmenu_=NULL;

	//!
	QToolBar *toolbar_ = NULL;

	//!PROTO �ļ�Ŀ¼��TAB
	ProtoDirTab *proto_dir_tab_ = NULL;

	//!EXCEL Ŀ¼��TAB
	ExcelDirTab *excel_dir_tab_ = NULL;

	//!Outer Ŀ¼��TAB
	OuterDirTab *outer_dir_tab_ = NULL;

	//!List��TABLE�����ڣ����������Ϣ��ʾ�û���
	QTableWidget *info_widget_;

	//!��ǰ����Ϣ��
	int cur_info_row_ = 0;
};

#endif // VISIONMAINWND_H
