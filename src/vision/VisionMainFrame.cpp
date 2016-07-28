#include "stdafx.h"
#include "ProtoDirTab.h"
#include "OuterDirTab.h"
#include "ExcelDirTab.h"
#include "SelectEachDirDialog.h"
#include "SelectOneDirDialog.h"
#include "VisionMainFrame.h"

//ʵ��ָ��
VisionMainFrame *VisionMainFrame::instace_ = NULL;

VisionMainFrame::VisionMainFrame(QWidget *parent)
	: QMainWindow(parent)
{
	setup_ui();
	setup_action();

	out_info(PZ_INFO, QString::fromLocal8Bit("�ðɣ������������������ߵ�ͼ�꣬������Ӧ��Ҳ�Ͳ��ѣ���ͷ��á�"));
	out_info(PZ_INFO, QString::fromLocal8Bit("����Ϸ��Ƭβ���С�Eyes on me�������Ƶ���ɤ����������������ʱ�䣬��Ȼ����Ҳ�ܺÿ���"));
	out_info(PZ_INFO, QString::fromLocal8Bit("��˵��������̫���� ������������ȡ���� ��˵��������̫���� ������������ȡ���� ��˵��������̫���� ���������ȡ����"));
	out_info(PZ_INFO, QString::fromLocal8Bit("��ӭ��ʹ��������ߣ�����н�����߷���bug����ϵhunterhli����seiferan, yvanfyin,sailzeng��"));
	out_info(PZ_INFO, QString::fromLocal8Bit("��ӭ��ʹ��������ߣ�����淨��������ϵmooncake,renahrhuang��"));
}

VisionMainFrame::~VisionMainFrame()
{

}



void VisionMainFrame::setup_ui()
{
	if (this->objectName().isEmpty())
	{
		this->setObjectName(QStringLiteral("this"));
	}
	this->resize(1380, 960);

	//
	QWidget *central_widget = new QWidget(this);
	QVBoxLayout *vertical_layout = new QVBoxLayout(central_widget);
	vertical_layout->setSpacing(6);
	vertical_layout->setContentsMargins(11, 11, 11, 11);

	main_splitter_ = new QSplitter(central_widget);

	main_splitter_->setOrientation(Qt::Vertical);


	main_tab_widget_ = new QTabWidget(main_splitter_);
	main_tab_widget_->setTabPosition(QTabWidget::South);


	
	proto_dir_tab_ = new ProtoDirTab(main_tab_widget_);
	main_tab_widget_->addTab(proto_dir_tab_, QString::fromLocal8Bit("ͳ�ƹؿ�"));
	excel_dir_tab_ = new ExcelDirTab(main_tab_widget_);
	main_tab_widget_->addTab(excel_dir_tab_, QString::fromLocal8Bit("�ؿ��Ƚ�"));
	outer_dir_tab_ = new OuterDirTab(main_tab_widget_);
	main_tab_widget_->addTab(outer_dir_tab_, QString::fromLocal8Bit("Ԫ������"));

	main_splitter_->addWidget(main_tab_widget_);

	//��Ϣ����
	info_widget_ = new QTableWidget(main_splitter_);
	info_widget_->setColumnCount(3);
	info_widget_->setRowCount(MAX_TIPS_INFO_NUMBER);
	main_splitter_->addWidget(info_widget_);
	QStringList headers;
	headers << QString::fromLocal8Bit("ʱ��") << QString::fromLocal8Bit("����") << QString::fromLocal8Bit("��Ϣ");
	info_widget_->setHorizontalHeaderLabels(headers);
	info_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
	info_widget_->setSelectionBehavior(QAbstractItemView::SelectRows);
	info_widget_->verticalHeader()->setVisible(false);
	info_widget_->setColumnWidth(0, 200);
	info_widget_->setColumnWidth(1, 60);
	info_widget_->setColumnWidth(2, 800);
	for (size_t i = 0; i < MAX_TIPS_INFO_NUMBER; ++i)
	{
		info_widget_->setRowHeight(i, 18);
	}

	vertical_layout->addWidget(main_splitter_);

	main_splitter_->setStretchFactor(0, 15);
	main_splitter_->setStretchFactor(1, 4);

	this->setCentralWidget(central_widget);

	main_tab_widget_->setCurrentIndex(0);

	this->setWindowTitle(QString::fromLocal8Bit("We6PuzzleDesigner ������"));

	m_alarm = new QSound(".\\res\\alarm\\alarm.wav", this);

	QMetaObject::connectSlotsByName(this);
}



void VisionMainFrame::setup_action()
{
	//ͼ������
	//https://www.iconfinder.com/icons/474135/construction_home_spanner_tool_wrench_icon#size=32
	act_allinone_ = new QAction(QIcon(".\\res\\icon\\allinone.png"),
							QString::fromLocal8Bit("ѡ��ALLINONEĿ¼���ʺ�����Ŀ¼����һ��Ŀ¼�µĴ���."),
							this);
	connect(act_allinone_,
			&QAction::triggered,
			this,
			&VisionMainFrame::allinone_process);

	act_each_ = new QAction(QIcon(".\\res\\icon\\each.png"),
								QString::fromLocal8Bit("�ֱ�ѡ��ÿ��Ŀ¼���ʺ�Ŀ¼���ڲ�ͬ�ط�."),
								this);
	connect(act_each_,
			&QAction::triggered,
			this,
			&VisionMainFrame::eachdir_process);

	act_exit_ = new QAction(QIcon(".\\res\\icon\\exit.png"),
							QString::fromLocal8Bit("�˳�"),
							this);
	connect(act_exit_,
			&QAction::triggered,
			this,
			&VisionMainFrame::exit_appliaction);

	/* �˵�����ʱ�����������
	m_menubar = new QMenuBar(this);
	m_menubar->setGeometry(QRect(0, 0, 1242, 23));
	m_mainmenu = new QMenu(QString::fromLocal8Bit("�ļ�"),m_menubar);
	this->setMenuBar(m_menubar);

	m_menubar->addAction(m_mainmenu->menuAction());
	m_mainmenu->addAction(m_act_open);
	m_mainmenu->addAction(m_act_save);
	m_mainmenu->addAction(m_act_saveas);
	m_mainmenu->addSeparator();
	m_mainmenu->addAction(m_act_exit);
	*/

	toolbar_ = new QToolBar(this);
	toolbar_->addAction(act_allinone_);
	toolbar_->addAction(act_each_);


	toolbar_->addSeparator();
	toolbar_->addAction(act_exit_);
	this->addToolBar(Qt::TopToolBarArea, toolbar_);
}

//
void VisionMainFrame::eachdir_process()
{
	int ret = 0;
	SelectEachDirDialog dialog(instace_);
	if (QDialog::Accepted == dialog.exec())
	{
		QString proto_path;
		QString excel_path;
		QString outer_path;
		QStringList import_list;
		QStringList tips_ary;
		dialog.get_path_str(proto_path, excel_path, outer_path, import_list);
		ret = Biko_Read_Config::instance()->init_read_all(proto_path,
														  excel_path,
														  outer_path,
														  import_list,
														  tips_ary);
		if (0 != ret)
		{

		}
		else
		{

		}
	}
}

//
void VisionMainFrame::allinone_process()
{
	int ret = 0;
	SelectOneDirDialog dialog(instace_);
	if (QDialog::Accepted == dialog.exec())
	{
		
		QString allinone_path;
		QStringList tips_ary;
		dialog.get_path_str(allinone_path);
		ret = Biko_Read_Config::instance()->init_read_all2(allinone_path,
 														   tips_ary);
		if (0 != ret)
		{

		}
		else
		{

		}
	}
}


//�˳�APP
void VisionMainFrame::exit_appliaction()
{
	QApplication::instance()->quit();
}



//�����Ϣ
void VisionMainFrame::out_info(PZ_TIP_LEVEL level, const QString &out_info)
{
	if (cur_info_row_ >= MAX_TIPS_INFO_NUMBER)
	{
		//ɾ���������һ����¼
		info_widget_->removeRow(0);
		cur_info_row_--;
		info_widget_->insertRow(MAX_TIPS_INFO_NUMBER - 1);
		info_widget_->setRowHeight(MAX_TIPS_INFO_NUMBER - 1, 18);
	}

	QColor color(Qt::red);
	QDateTime cur_time = QDateTime::currentDateTime();
	info_widget_->setItem(cur_info_row_, 0, new QTableWidgetItem(cur_time.toString("yyyy-dd-MM HH:mm:ss")));

	switch (level)
	{
	case PZ_DEBUG:
		color = Qt::green;
		info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("����")));
		break;
	case PZ_INFO:
		color = Qt::blue;
		info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("��Ϣ")));
		break;
	case PZ_WARNNING:
		color = Qt::magenta;
		info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("����")));
		m_alarm->play();
		break;
	case PZ_ERROR:
		color = Qt::red;
		info_widget_->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("����")));
		m_alarm->play();
		break;
	}
	info_widget_->setItem(cur_info_row_, 2, new QTableWidgetItem(out_info));
	info_widget_->setCurrentCell(cur_info_row_, 1);

	QTableWidgetItem *table_item = NULL;
	table_item = info_widget_->item(cur_info_row_, 0);
	table_item->setTextColor(color);
	table_item = info_widget_->item(cur_info_row_, 1);
	table_item->setTextColor(color);
	table_item = info_widget_->item(cur_info_row_, 2);
	table_item->setTextColor(color);

	cur_info_row_++;
}

//
void VisionMainFrame::select_tab(int tab_id)
{
	main_tab_widget_->setCurrentIndex(tab_id);
}



//����ʵ��ָ��
void VisionMainFrame::set_instance(VisionMainFrame *inst)
{
	instace_ = inst;
}
//��ȡʵ��ָ��
VisionMainFrame *VisionMainFrame::instance()
{
	return instace_;
}

bool VisionMainFrame::read_config(const QString &section,
								  const QString &key,
								  QVariant &data)
{
	QFile json_file(".\\cfg\\local_config.json");
	bool rd_ok = json_file.open(QIODevice::ReadOnly);
	if (!rd_ok)
	{
		return false;
	}

	//��ȡJson
	QByteArray b = json_file.readAll();
	QJsonParseError json_error;
	QJsonDocument json_doc = QJsonDocument::fromJson(b, &json_error);
	if (json_error.error != QJsonParseError::NoError)
	{
		return false;
	}

	if (!json_doc.isObject())
	{
		return false;
	}

	QVariantMap section_result = json_doc.toVariant().toMap();
	QVariantMap::const_iterator iter_find = section_result.find(section);
	if (section_result.end() == iter_find)
	{
		return false;
	}
	QVariantMap key_result = iter_find->toMap();
	iter_find = key_result.find(key);
	if (key_result.end() == iter_find)
	{
		return false;
	}

	//�����ҵ���
	data = iter_find->toString();
	return true;
}

//!д������
bool VisionMainFrame::write_config(const QString &section,
								   const QString &key,
								   const QVariant &data)
{
	QFile json_file(".\\cfg\\local_config.json");
	bool rd_ok = json_file.open(QIODevice::ReadWrite);
	if (!rd_ok)
	{
		QMessageBox::critical(VisionMainFrame::instance(),
							  QString::fromLocal8Bit("����"),
							  QString::fromLocal8Bit("���������ļ�[.\\cfg\\local_config.json]�޷���ȷ��ȡ,����"));
		return false;
	}

	//��ȡJson
	QByteArray b = json_file.readAll();
	QJsonParseError json_error;
	QJsonDocument json_doc = QJsonDocument::fromJson(b, &json_error);


	QVariantMap key_result;

	QVariantMap section_result = json_doc.toVariant().toMap();
	QVariantMap::const_iterator iter_find = section_result.find(section);
	if (section_result.end() == iter_find)
	{
		key_result.insert(key, data);
		section_result.insert(section, key_result);
	}
	else
	{
		key_result = iter_find->toMap();
		key_result[key] = data;
		section_result.insert(section, key_result);
	}

	QJsonDocument write_doc = QJsonDocument::fromVariant(section_result);
	json_file.resize(0);
	qint64 wt_ok = json_file.write(write_doc.toJson());
	if (wt_ok <= 0)
	{
		QMessageBox::critical(VisionMainFrame::instance(),
							  QString::fromLocal8Bit("����"),
							  QString::fromLocal8Bit("���������ļ�[.\\cfg\\local_config.json]�޷���ȷд��,����"));
		return false;
	}
	json_file.close();

	return true;
}

