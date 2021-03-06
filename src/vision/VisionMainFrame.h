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


    //!输出信息
    void out_info(PZ_TIP_LEVEL level, const QString &out_info);
    //!
    void out_tips_ary(const QStringList &tips_ary);

protected:

    //!整个UI处理
    void setup_ui();

    //!菜单处理
    void setup_action();

    //SLOT
protected:
    //!每个目录都处理
    void eachdir_process();

    //!ALL IN ONE目录的处理
    void allinone_process();

    //!程序退出
    void exit_appliaction();

    //!PROTO TAB页面选择所有的Message
    void select_all_message();

    //!PROTO TAB页面所有的Message都不选
    void select_none_message();

	//!读取所有选择的PROTO文件
	void proto_read_all();

	//!根据.proto文件，对所有没有对应EXCEL文件的生成相应的EXCEL文件
	void create_excel();
    
    //清理OUTER信息区
    void clear_info();

    void save_info_to_txt();

public:

    //设置实例指针
    static void set_instance(VisionMainFrame *inst);
    //获取实例指针
    static VisionMainFrame *instance();

    //!读取配置
    static bool read_config(const QString &section,
                            const QString &key,
                            QVariant &data);

    //!写入配置
    static bool write_config(const QString &section,
                             const QString &key,
                             const QVariant &data);

protected:

    //实例指针
    static VisionMainFrame *instace_;

    //最大消息数量
    static const size_t MAX_TIPS_INFO_NUMBER = 512;

private:

    //!Action

    QAction *act_allinone_ = NULL;
    QAction *act_eachdir_ = NULL;

    //!
    QAction *act_select_all_ = NULL;
    QAction *act_select_none_ = NULL;
    QAction *act_read_all_ = NULL;
	QAction *act_create_excel_ = NULL;
    QAction *act_clear_info_ = NULL;
    QAction *act_save_info_ = NULL;
    QAction *act_exit_ = NULL;

    //!分割线
    QSplitter *main_splitter_ = NULL;

    //!告警声音
    QSound *m_alarm = NULL;

    //!菜单的menu bar
    QMenuBar *menubar_ = NULL;
    //!主菜单
    QMenu *mainmenu_ = NULL;

    //!
    QToolBar *toolbar_ = NULL;

    //!PROTO 文件目录的TAB
    IllusionWidget *illlusion_widget_ = NULL;

    //!List（TABLE）窗口，用于输出信息提示用户。
    QTableWidget *info_widget_;

    //!当前的信息行
    int cur_info_row_ = 0;
};

#endif // VISIONMAINWND_H
