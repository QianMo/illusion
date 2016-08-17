#ifndef VISIONMAINWND_H
#define VISIONMAINWND_H


class IllusionWidget;

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


	//!�����Ϣ
	void out_info(PZ_TIP_LEVEL level, const QString &out_info);
	//!
	void out_tips_ary(const QStringList &tips_ary);

protected:

	//!����UI����
	void setup_ui();

	//!�˵�����
	void setup_action();

	//SLOT
protected:


	
	//!ÿ��Ŀ¼������
	void eachdir_process();

	//!ALL IN ONEĿ¼�Ĵ���
	void allinone_process();

	//!�����˳�
	void exit_appliaction();

	//!PROTO TABҳ��ѡ�����е�Message
	void proto_select_all();

	//!PROTO TABҳ�����е�Message����ѡ
	void proto_select_none();

	//!PROTO TABҳ��
	void proto_read_all();
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

protected:

	//ʵ��ָ��
	static VisionMainFrame *instace_;

	//�����Ϣ����
	static const size_t MAX_TIPS_INFO_NUMBER = 128;

private:

	//!Action
	
	QAction *act_allinone_ = NULL;
	QAction *act_eachdir_ = NULL;
	
	//!
	QAction *act_proto_selectall_ = NULL;
	QAction *act_proto_selectnone_ = NULL;
	QAction *act_proto_readall_ = NULL;
	
	QAction *act_exit_ = NULL;

//!�ָ���
	QSplitter *main_splitter_ = NULL;

	//!�澯����
	QSound *m_alarm = NULL;

	//!�˵���menu bar
	QMenuBar *menubar_ = NULL;
	//!���˵�
	QMenu *mainmenu_=NULL;

	//!
	QToolBar *toolbar_ = NULL;

	//!PROTO �ļ�Ŀ¼��TAB
	IllusionWidget *illlusion_widget_ = NULL;

	//!List��TABLE�����ڣ����������Ϣ��ʾ�û���
	QTableWidget *info_widget_;

	//!��ǰ����Ϣ��
	int cur_info_row_ = 0;
};

#endif // VISIONMAINWND_H
