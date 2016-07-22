#include "stdafx.h"
#include "VisionMainFrame.h"

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

	config_tab_ = new ConfigChessBoardTab(main_tab_widget_);
	main_tab_widget_->addTab(config_tab_, QString::fromLocal8Bit("�ؿ����"));
	simulator_tab_ = new SimulatorTab(main_tab_widget_);
	main_tab_widget_->addTab(simulator_tab_, QString::fromLocal8Bit("�ܷ���"));
	statist_tab_ = new StatistPuzzleInfoTab(main_tab_widget_);
	main_tab_widget_->addTab(statist_tab_, QString::fromLocal8Bit("ͳ�ƹؿ�"));
	comparedir_puzzle_tab_ = new CompareDirPuzzleTab(main_tab_widget_);
	main_tab_widget_->addTab(comparedir_puzzle_tab_, QString::fromLocal8Bit("�ؿ��Ƚ�"));
	elements_protperties_tab_ = new ElementsPropertiesTab(main_tab_widget_);
	main_tab_widget_->addTab(elements_protperties_tab_, QString::fromLocal8Bit("Ԫ������"));

	main_splitter_->addWidget(main_tab_widget_);

	//��Ϣ����
	m_info_widget = new QTableWidget(main_splitter_);
	m_info_widget->setColumnCount(3);
	m_info_widget->setRowCount(MAX_TIPS_INFO_NUMBER);
	main_splitter_->addWidget(m_info_widget);
	QStringList headers;
	headers << QString::fromLocal8Bit("ʱ��") << QString::fromLocal8Bit("����") << QString::fromLocal8Bit("��Ϣ");
	m_info_widget->setHorizontalHeaderLabels(headers);
	m_info_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_info_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_info_widget->verticalHeader()->setVisible(false);
	m_info_widget->setColumnWidth(0, 200);
	m_info_widget->setColumnWidth(1, 60);
	m_info_widget->setColumnWidth(2, 800);
	for (size_t i = 0; i < MAX_TIPS_INFO_NUMBER; ++i)
	{
		m_info_widget->setRowHeight(i, 18);
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
	// https://www.iconfinder.com/icons/196690/edit_pencil_icon#size=32

	act_new_ = new QAction(QIcon(".\\res\\icon\\new.png"),
						   QString::fromLocal8Bit("�½����ض���Ҫ���½���ÿ�����´򿪲�����"),
						   this);
	connect(act_new_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::new_cfg_file);

	act_open_ = new QAction(QIcon(".\\res\\icon\\open.png"),
							QString::fromLocal8Bit("��(CTRL+O)"),
							this);
	connect(act_open_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::open_cfg_file);
	act_save_ = new QAction(QIcon(".\\res\\icon\\save.png"),
							QString::fromLocal8Bit("����(Ctrl+S)"),
							this);
	connect(act_save_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::save_cfg_file);
	act_saveas_ = new QAction(QIcon(".\\res\\icon\\saveas.png"),
							  QString::fromLocal8Bit("���Ϊ(F12)"),
							  this);
	connect(act_saveas_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::saveas_cfg_file);

	act_undo_ = new QAction(QIcon(".\\res\\icon\\undo.png"),
							QString::fromLocal8Bit("����(Ctrl+Z)"),
							this);
	connect(act_undo_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::go_back);

	act_redo_ = new QAction(QIcon(".\\res\\icon\\redo.png"),
							QString::fromLocal8Bit("����(Ctrl+Y)"),
							this);
	connect(act_redo_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::go_front);

	act_statist_ = new QAction(QIcon(".\\res\\icon\\statist.png"),
							   QString::fromLocal8Bit("ͳ��һ��Ŀ¼�µ������ļ�"),
							   this);
	connect(act_statist_,
			&QAction::triggered,
			statist_tab_,
			&StatistPuzzleInfoTab::statist_puzzle_dir);

	act_statresult_ = new QAction(QIcon(".\\res\\icon\\statresult.png"),
								  QString::fromLocal8Bit("����ͳ�ƵĽ����EXCEL�ļ�������EXCEL�ǳ�����"
														 "���ӿ��Ը�΢���ᵥ������Ϊ����Ż����Ѫ��"),
								  this);
	connect(act_statresult_,
			&QAction::triggered,
			statist_tab_,
			&StatistPuzzleInfoTab::save_statist_result);

	act_surprise_ = new QAction(QIcon(".\\res\\icon\\surprise.png"),
								QString::fromLocal8Bit("surprise me. �ⲻ�ǡ���ʳ�ܶ�Ա����̨�ʣ����ǲ���һ���������������."),
								this);
	connect(act_surprise_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::surprise_puzzle);

	act_checkdir = new QAction(QIcon(".\\res\\icon\\checkdir.png"),
							   QString::fromLocal8Bit("���Ŀ¼�����е������ļ�"),
							   this);
	connect(act_checkdir,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::on_check_dir);

	act_fill_ = new QAction(QIcon(".\\res\\icon\\fill.png"),
							QString::fromLocal8Bit("һ������������"),
							this);
	connect(act_fill_,
			&QAction::triggered,
			config_tab_,
			&ConfigChessBoardTab::fill_random);

	act_compare_ = new QAction(QIcon(".\\res\\icon\\compare.png"),
							   QString::fromLocal8Bit("�Ƚ�����Ŀ¼���ļ�"),
							   this);


	act_special_ = new QAction(QIcon(".\\res\\icon\\special.png"),
							   QString::fromLocal8Bit("�������"),
							   this);


	act_exit_ = new QAction(QIcon(".\\res\\icon\\exit.png"),
							QString::fromLocal8Bit("�˳�"),
							this);
	connect(act_exit_,
			&QAction::triggered,
			this,
			&PuzzleMainFrame::exit_appliaction);



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

	m_toolbar = new QToolBar(this);
	m_toolbar->addAction(act_new_);
	m_toolbar->addAction(act_open_);
	m_toolbar->addAction(act_save_);
	m_toolbar->addAction(act_saveas_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_undo_);
	m_toolbar->addAction(act_redo_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_statist_);
	m_toolbar->addAction(act_statresult_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_checkdir);

	m_toolbar->addSeparator();
	m_toolbar->addAction(act_surprise_);
	m_toolbar->addSeparator();
	m_toolbar->addAction(act_compare_);
	m_toolbar->addAction(act_compresult_);


	m_toolbar->addSeparator();
	m_toolbar->addAction(act_special_);
	m_toolbar->addAction(act_fill_);

	m_toolbar->addSeparator();
	m_toolbar->addAction(act_exit_);
	this->addToolBar(Qt::TopToolBarArea, m_toolbar);
}

//�˳�APP
void VisionMainFrame::exit_appliaction()
{
	//QAction *evt_act = qobject_cast<QAction *>(sender());
	QApplication::instance()->quit();
}



//�����Ϣ
void VisionMainFrame::out_info(PZ_TIP_LEVEL level, const QString &out_info)
{
	if (cur_info_row_ >= MAX_TIPS_INFO_NUMBER)
	{
		//ɾ���������һ����¼
		m_info_widget->removeRow(0);
		cur_info_row_--;
		m_info_widget->insertRow(MAX_TIPS_INFO_NUMBER - 1);
		m_info_widget->setRowHeight(MAX_TIPS_INFO_NUMBER - 1, 18);
	}

	QColor color(Qt::red);
	QDateTime cur_time = QDateTime::currentDateTime();
	m_info_widget->setItem(cur_info_row_, 0, new QTableWidgetItem(cur_time.toString("yyyy-dd-MM HH:mm:ss")));

	switch (level)
	{
	case PZ_DEBUG:
		color = Qt::green;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("����")));
		break;
	case PZ_INFO:
		color = Qt::blue;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("��Ϣ")));
		break;
	case PZ_WARNNING:
		color = Qt::magenta;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("����")));
		m_alarm->play();
		break;
	case PZ_ERROR:
		color = Qt::red;
		m_info_widget->setItem(cur_info_row_, 1, new QTableWidgetItem(QString::fromLocal8Bit("����")));
		m_alarm->play();
		break;
	}
	m_info_widget->setItem(cur_info_row_, 2, new QTableWidgetItem(out_info));
	m_info_widget->setCurrentCell(cur_info_row_, 1);

	QTableWidgetItem *table_item = NULL;
	table_item = m_info_widget->item(cur_info_row_, 0);
	table_item->setTextColor(color);
	table_item = m_info_widget->item(cur_info_row_, 1);
	table_item->setTextColor(color);
	table_item = m_info_widget->item(cur_info_row_, 2);
	table_item->setTextColor(color);

	cur_info_row_++;
}

//
void VisionMainFrame::select_tab(int tab_id)
{
	main_tab_widget_->setCurrentIndex(tab_id);
}

//
VisionMainFrame *VisionMainFrame::configboard_tab()
{
	return config_tab_;
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
								  const QString &defult_data,
								  QString &data
)
{
	data = defult_data;
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
								   const QString &data)
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

