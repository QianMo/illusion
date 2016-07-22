#ifndef VISIONMAINWND_H
#define VISIONMAINWND_H

#include <QtWidgets/QMainWindow>


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

	//!�����˳�
	void exit_appliaction();

public:

	//����ʵ��ָ��
	static void set_instance(VisionMainFrame *inst);
	//��ȡʵ��ָ��
	static VisionMainFrame *instance();

	//!��ȡ����
	static bool read_config(const QString &section,
							const QString &key,
							const QString &defult_data,
							QString &data);

	//!д������
	static bool write_config(const QString &section,
							 const QString &key,
							 const QString &data);

	//ʵ��ָ��
	static VisionMainFrame *instace_;

private:

	//!Action
	QAction *act_new_ = NULL;
	QAction *act_open_ = NULL;
	QAction *act_save_ = NULL;
	QAction *act_saveas_ = NULL;
	QAction *act_undo_ = NULL;
	QAction *act_redo_ = NULL;
	QAction *act_statist_ = NULL;
	QAction *act_statresult_ = NULL;
	QAction *act_checkdir = NULL;
	QAction *act_surprise_ = NULL;
	QAction *act_compare_ = NULL;
	QAction *act_compresult_ = NULL;
	QAction *act_special_ = NULL;
	QAction *act_fill_ = NULL;
	QAction *act_exit_ = NULL;


	//!�ָ���
	QSplitter *main_splitter_ = NULL;
	//!�����ڵ�TAB
	QTabWidget *main_tab_widget_ = NULL;

	//!�澯����
	QSound *m_alarm = NULL;

	//!�˵���menu bar
	QMenuBar *m_menubar;
	//!���˵�
	QMenu *m_mainmenu;

	//!
	QToolBar *m_toolbar = NULL;


	//!List��TABLE�����ڣ����������Ϣ��ʾ�û���
	QTableWidget *m_info_widget;

	//!��ǰ����Ϣ��
	int cur_info_row_ = 0;
};

#endif // VISIONMAINWND_H
