#include "biko_predefine.h"
#include "biko_read_config.h"


char Biko_Read_Config::REPEATED_STRING[] = "list_data";

//����ĵ���ʵ��
Biko_Read_Config *Biko_Read_Config::instance_ = NULL;

//
Biko_Read_Config::Biko_Read_Config()
{
}


Biko_Read_Config::~Biko_Read_Config()
{
}


Biko_Read_Config *Biko_Read_Config::instance()
{
    if (instance_ == NULL)
    {
        instance_ = new Biko_Read_Config();
    }
    return instance_;
}

void Biko_Read_Config::clean_instance()
{
    if (instance_)
    {
        delete instance_;
        instance_ = NULL;
    }
}

//!���е�Ŀ¼����һ��Ŀ¼�µĿ�ݴ���ʽ
int Biko_Read_Config::init_read_all2(const QString &allinone_dir,
										 QString &error_tips)
{
	QString outer_dir = allinone_dir + "/outer";
	return init_read_all(allinone_dir + "/excel",
						 allinone_dir + "/proto",
						 &outer_dir,
						 error_tips);
}

//��ȡexcel_dirĿ¼�����е�EXCEL�ļ�������proto_dirĿ¼�µ�meta�ļ������䣬ת����λ���ļ������outer_dirĿ¼
int Biko_Read_Config::init_read_all(const QString &excel_dir,
									    const QString &proto_dir,
										const QString *outer_dir,
										QString &error_tips)
{
	int ret = 0;
	excel_path_.setPath(excel_dir);
	if (false == excel_path_.exists())
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
			arg(excel_dir);
		return -1;
	}
	//��ȡ.xls , .xlsx �ļ�
	QStringList filters;
	filters << "*.xls" << "*.xlsx";
	excel_fileary_ = excel_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (excel_fileary_.size() <= 0)
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�Excel�ļ������������").
			arg(excel_dir);
		return -1;
	}

	ret = init_read(proto_dir,outer_dir,error_tips);
	if (ret != 0)
	{
		return -1;
	}
    return 0;
}

//��ʼ����׼����ȡһ��EXCEL�ļ���ת��Ϊ�����ļ�
int Biko_Read_Config::init_read_one(const QString &excel_file,
										const QString *excel_table_name,
										const QString &proto_dir,
										const QString *outer_dir,
										QString &error_tips)
{
	int ret = 0;

	QFileInfo file_info(excel_file);
	if (!file_info.exists())
	{
		error_tips = QString::fromLocal8Bit("EXCEL�ļ�[%1]�������ڣ����顣").
			arg(excel_file);
		return -1;
	}
	excel_fileary_ << file_info;
	excel_table_name_.clear();
	if (excel_table_name)
	{
		excel_table_name_ = *excel_table_name;
	}

	ret = init_read(proto_dir, outer_dir,error_tips);
	if (ret != 0)
	{
		return -1;
	}



	return 0;
}

//��ȡEXCEL�����������
int Biko_Read_Config::init_read(const QString &proto_dir, 
									const QString *outer_dir,
									QString &error_tips)
{
	int ret = 0;
	//
	ret = init_protodir(proto_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}

	ret = init_outdir(outer_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}

	bool bret = ils_excel_file_.initialize(false);
	if (false == bret)
	{
		error_tips = QString::fromLocal8Bit("OLE��������EXCEL��ʵ��OLE��ȡEXCEL���밲װ��EXCEL��");
		return -1;
	}


	return 0;
}

//��ʼ��.proto�ļ�Ŀ¼����ȡ�������е�proto�ļ�
int Biko_Read_Config::init_protodir(const QString &proto_dir,
									QString &error_tips)
{
	int ret = 0;
	proto_path_.setPath(proto_dir);;
	if (false == proto_path_.exists())
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
			arg(proto_dir);
		return -1;
	}
	//��ȡ.proto �ļ�
	QStringList filters;
	filters << "*.proto";
	proto_fileary_ = proto_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (proto_fileary_.size() <= 0)
	{
		error_tips = QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�protobuf meta(.proto)�ļ������������").
			arg(proto_dir);
		return -1;
	}
	ils_proto_reflect_.map_path(proto_path_.path().toStdString());

	//�������е�.proto �ļ�
	for (int i = 0; i < proto_fileary_.size(); ++i)
	{

	}
	return 0;
}

int Biko_Read_Config::init_outdir(const QString *outer_dir,
									  QString &error_tips)
{
	QString out_path,path_str;
	if (outer_dir)
	{
		out_path = *outer_dir;
	}
	else
	{
		out_path = ".";
	}
	path_str = out_path;
	path_str += "/log";
	out_log_path_.setPath(path_str);
	if (false == out_log_path_.exists())
	{
		if (false == out_log_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	//db3��·��û�п��Դ���
	path_str = out_path;
	path_str += "/db3";
	out_db3_path_.setPath(path_str);
	if (false == out_db3_path_.exists())
	{
		if (false == out_db3_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	//pbc��·��û�п��Դ���
	path_str = out_path;
	path_str += "/pbc";
	out_pbc_path_.setPath(path_str);
	if (false == out_pbc_path_.exists())
	{
		if (false == out_pbc_path_.mkpath(path_str))
		{
			return -1;
		}
	}
	return 0;
}

//
void Biko_Read_Config::finalize()
{
    clear();

    if (ils_excel_file_.is_open())
    {
        ils_excel_file_.close();
        ils_excel_file_.finalize();
    }

    return;
}

//�������еĶ�ȡ����
void Biko_Read_Config::clear()
{
    file_cfg_map_.clear();
}

//��ɨ����߲�����EXCEL�ļ�������һ�ζ�ȡ
int Biko_Read_Config::read_excel(QString &error_tips)
{
	//
	int ret = 0;
	if (!excel_table_name_.isEmpty() && excel_fileary_.size()== 1)
	{
		return read_one_excel(excel_fileary_[0].filePath(),
							  &excel_table_name_,
							  error_tips);
	}
	else
	{
		//����д���ˣ�
		Q_ASSERT(excel_table_name_.isEmpty());
		for (int i = 0; i < excel_fileary_.size(); ++i)
		{
			ret = read_one_excel(excel_fileary_[i].filePath(),
								 NULL,
								 error_tips);
			if (0 != ret)
			{
				return ret;
			}
		}
	}
	return 0;
}

//��ȡһ��EXCEL�ļ�������ƶ��˱��ֻ��ȡ�ض����
int Biko_Read_Config::read_one_excel(const QString &open_file,
									     const QString *excel_table_name,
									     QString &error_tips)
{
    clear();

    bool bret = ils_excel_file_.open(open_file);
    //Excel�ļ���ʧ��
    if (bret != true)
    {
        return -1;
    }
    //
    qDebug() << "Dream excecl file have sheet num["
             << ils_excel_file_.sheetsCount()
             << "].\n";

    //������
    if (ils_excel_file_.hasSheet("TABLE_CONFIG") == false ||
        ils_excel_file_.hasSheet("ENUM_CONFIG") == false)
    {
        //
        error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�����ܶ�ȡ�����ñ�[TABLE_CONFIG]"
											" or [ENUM_CONFIG]"
											"�������ּ���򿪡�!");
        return -1;
    }

    //file_cfg_map_[open_file] = excel_data;
    EXCEL_FILE_DATA excel_data;
    auto result = file_cfg_map_.insert(std::make_pair(open_file, excel_data));
    if (!result.second)
    {
        return -1;
    }

    //
    EXCEL_FILE_DATA &xls_data = (*result.first).second;
    int ret = read_table_enum(xls_data);
    if (0 != ret)
    {
        error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�ļ��е�[ENUM_CONFIG]����ȷ�������ּ���򿪡�!");
        return ret;
    }

    //
    ret = read_table_config(xls_data, error_tips);
    if (0 != ret)
    {
        error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�ļ��е�TABLE_CONFIG����ȷ�������ּ���򿪡�!");
        return ret;
    }

	if (excel_table_name && ils_excel_file_.hasSheet(*excel_table_name) == false)
	{
		error_tips = QString::fromLocal8Bit("û��һ�ű�񱻶�ȡ��!�����õĶ�ȡ���[%1]Ӧ�ò�����").
			arg(*excel_table_name);
		return -1;
	}

	bool already_read = false;
    auto iter_tmp = xls_data.xls_table_cfg_.begin();
    for (; iter_tmp != xls_data.xls_table_cfg_.end(); ++iter_tmp)
    {
		//�����ʶ��table name��ֻ��ȡ���table
		if (excel_table_name && *excel_table_name != iter_tmp->second.excel_table_name_)
		{
			continue;
		}
		already_read = true;

		if (iter_tmp->second.save_pb_config_.isEmpty() == false)
		{
			google::protobuf::Message *list_msg = NULL;
			ret = read_sheet_pbcdata(iter_tmp->second, list_msg, error_tips);
			if (0 != ret)
			{
				return ret;
			}

			ret = save_to_protocfg(iter_tmp->second, list_msg, error_tips);
			if (0 != ret)
			{
				return ret;
			}
		}
    }
	//���û�ж�ȡ,������ǰ�����˱���Ƿ����
	if (!already_read)
	{
		error_tips = QString::fromLocal8Bit("û��һ�ű�񱻶�ȡ��!ԭ�������ҵ�ȷ������");
		return -1;
	}
    return 0;
}

int Biko_Read_Config::read_proto_file(const QString & proto_file,
									  QStringList & tips_info)
{
	int ret = 0;
	const google::protobuf::FileDescriptor *file_desc = NULL;
	ret = ils_proto_reflect_.import_file(proto_file.toStdString(), 
										 file_desc);
	if (ret != 0 || NULL == file_desc )
	{
		return -1;
	}



	//�������е�Message���ҳ�����Ҫ����ģ�
	int msg_count =file_desc->message_type_count();
	for (int i = 0; i < msg_count; ++i)
	{
		const google::protobuf::Descriptor *listmsg_desc = file_desc->message_type(i);
		
		bool illusion_msg = mo.GetExtension(illusion::illusion_message);
		if (illusion_msg && listmsg_desc->field_count() > 0)
		{
			
			
		}
		
	}

	return 0;
}




//��ȡ���е�ö��ֵ
int Biko_Read_Config::read_table_enum(EXCEL_FILE_DATA &file_cfg_data)
{
    //ǰ�������
    bool bret =  ils_excel_file_.loadSheet("ENUM_CONFIG");
    if (bret == false)
    {
        return -1;
    }

    //��Ӧ����
    int row_count = ils_excel_file_.row_count();
    int col_count = ils_excel_file_.column_count();
    qDebug() << "ENUM_CONFIG table have col_count = "
             << col_count
             << " row_count ="
             << row_count
             << "\n";

    //ע�����е��±궼�Ǵ�1��ʼ��
    const long COL_ENUM_KEY = 1;
    const long COL_ENUM_VALUE = 2;
    const QChar ENUM_FIRST_CHAR = '[';

    size_t read_enum = 0;
    //��ȡ���е���
    for (long i = 1; i <= row_count; ++i)
    {

        long   row_no = i;
        //�ַ���
        QString enum_key = ils_excel_file_.get_cell(row_no, COL_ENUM_KEY).toString();

        //�����һ���ַ���[
        if (enum_key[0] == ENUM_FIRST_CHAR )
        {
            QString enum_vlaue = ils_excel_file_.get_cell(row_no, COL_ENUM_VALUE).toString();
            file_cfg_data.xls_enum_[enum_key] = enum_vlaue;

            ++read_enum;
        }
    }
    return 0;
}


//��ȡ�������
int Biko_Read_Config::read_table_config(EXCEL_FILE_DATA &file_cfg_data,
											QString &error_tips)
{
    //ǰ�������
    bool bret = ils_excel_file_.loadSheet("TABLE_CONFIG");
    if (bret == false)
    {
		error_tips = QString::fromLocal8Bit("��ѡ�������EXCEL�����ܶ�ȡ�����ñ�[TABLE_CONFIG]"
											"�������ּ���򿪡�!");
        return -1;
    }

    long row_count = ils_excel_file_.row_count();
    long col_count = ils_excel_file_.column_count();
    qDebug() << "TABLE_CONFIG table have col_count = " << col_count << " row_count =" << row_count << "\n";

    //ע�����е��±궼�Ǵ�1��ʼ��
    const int COL_TC_KEY = 1;
    const int COL_TC_VALUE = 2;
	bool info_imperfect = false;
	QString cur_process_sheet;

    for (int row_no = 1; row_no <= row_count; ++row_no)
    {

        QString tc_key = ils_excel_file_.get_cell(row_no, COL_TC_KEY).toString();

        QString temp_value;
        TABLE_CONFIG tc_data;
		
		//����һ��������ƣ��Ͷ�ȡһ������
        if (tc_key == QString::fromLocal8Bit("��ȡ��SHEET����"))
        {
            tc_data.excel_table_name_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.excel_table_name_.isEmpty())
            {
                continue;
            }
			cur_process_sheet = tc_data.excel_table_name_;
			//���ʣ������ݲ�������Ϊ��������
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
                break;
            }

            tc_data.read_data_start_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.read_data_start_ <= 0)
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]����ʼ��ȡ���ֶδ����кŴ�1��ʼ��").
					arg(tc_data.excel_table_name_);
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
				break;
            }

            tc_data.pb_line_message_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
            if (tc_data.pb_line_message_.isEmpty())
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]�Ķ�Ӧ��Protobuf ��Ӧ����Message������д��").
					arg(tc_data.excel_table_name_);
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
				break;
            }

            tc_data.pb_fieldname_line_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toInt();
            if (tc_data.pb_fieldname_line_ <= 0)
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]�Ķ�Ӧ��Protobuf ��Ӧ����Message������д��").
					arg(tc_data.excel_table_name_);
                return -1;
            }
            ++row_no;
            if (row_no > row_count)
            {
				info_imperfect = true;
				break;
            }

			//PB �������ļ����֣�����Ϊ��
			tc_data.save_pb_config_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
			if (tc_data.save_pb_config_.isEmpty())
			{
				error_tips = QString::fromLocal8Bit("SHEET[%1]���뵼�ɵ���PB�����ļ�����ѡ��һ��").
					arg(cur_process_sheet);
				return -1;
			}
			++row_no;
			if (row_no > row_count)
			{
				info_imperfect = true;
				break;
			}
			tc_data.pb_list_message_ = ils_excel_file_.get_cell(row_no, COL_TC_VALUE).toString();
			if (false ==tc_data.save_pb_config_.isEmpty() && true == tc_data.pb_list_message_.isEmpty())
			{
				error_tips = QString::fromLocal8Bit("SHEET[%1]���Ҫ�󵼳�Protobuf�ļ���������д��Ӧ��List Message�ṹ����").
					arg(tc_data.excel_table_name_);
				return -1;
			}
			++row_no;
			//���һ�п��Բ������
			//if (row_no > row_count)
		
            auto result = file_cfg_data.xls_table_cfg_.insert(std::make_pair(tc_data.excel_table_name_, tc_data));
            if (false == result.second)
            {
				error_tips = QString::fromLocal8Bit("SHEET[%1]�������ظ�������").
					arg(cur_process_sheet);
                return -2;
            }
        }
        else
        {
            continue;
        }
    }

	if (info_imperfect)
	{
		error_tips = QString::fromLocal8Bit("SHEET[%1]��������Ϣȱʧ������").
			arg(cur_process_sheet);
		return -3;
	}
	
    return 0;
}




int Biko_Read_Config::read_sheet_pbcdata(TABLE_CONFIG &tc_data, 
											 google::protobuf::Message *&list_msg, 
											 QString & error_tips)
{
	int ret = 0;
	//���EXCEL�ļ����Ƿ���������
	if (ils_excel_file_.loadSheet(tc_data.excel_table_name_) == false)
	{
		return -3;
	}

	ret = ils_proto_reflect_.new_mesage(tc_data.pb_list_message_.toStdString(), list_msg);
	if (ret != 0)
	{
		return ret;
	}


	int line_count = ils_excel_file_.row_count();
	int col_count = ils_excel_file_.column_count();
	qDebug() << tc_data.excel_table_name_ <<
		" table have col_count = "
		<< col_count
		<< " row_count ="
		<< line_count
		<< "\n";


	//�����ʶ��pb�ֶ��еȣ�����ʵû����ô����
	if (tc_data.pb_fieldname_line_ > line_count || tc_data.read_data_start_ > line_count)
	{
		return -4;
	}

	QString field_name_string;
	for (int col_no = 1; col_no <= col_count; ++col_no)
	{
		field_name_string = ils_excel_file_.get_cell(tc_data.pb_fieldname_line_, col_no).toString();
		tc_data.proto_field_ary_.push_back(field_name_string);

		//��ε����ڸ�����Լ�Ҳֻ��˵������ˣ���Ϊrepeated ���ֶ���Ҫ��add message��
		//���Ծͱ����һ�γ��ֵĵط��ҳ���
		int find_pos = tc_data.proto_field_ary_[col_no - 1].lastIndexOf('.');
		if (find_pos != -1)
		{
			if (tc_data.firstshow_field_ == field_name_string)
			{
				tc_data.item_msg_firstshow_.push_back(true);
			}
			else
			{
				if (tc_data.firstshow_msg_.length() > 0 &&
					true == field_name_string.startsWith(tc_data.firstshow_msg_))
				{
					tc_data.item_msg_firstshow_.push_back(false);
				}
				else
				{
					tc_data.firstshow_field_ = field_name_string;
					tc_data.firstshow_msg_.append(field_name_string.constData(),
												  find_pos + 1);
					tc_data.item_msg_firstshow_.push_back(true);
				}
			}
		}
		else
		{
			tc_data.item_msg_firstshow_.push_back(false);
		}

	}

	std::vector<google::protobuf::Message *> field_msg_ary;
	std::vector<const google::protobuf::FieldDescriptor *> field_desc_ary;

	//��������������������������������Ά��µĴ���ֻ��Ϊ�˸����־������,EXCEFILENAE_TABLENAME.log
	QString xls_file_name;
	xls_file_name = ils_excel_file_.open_filename();
	QFileInfo xls_fileinfo(xls_file_name);
	QString file_basename = xls_fileinfo.baseName();

	QString log_file_name = file_basename;
	log_file_name += "_";
	log_file_name += tc_data.excel_table_name_;
	log_file_name += ".log";
	QString outlog_filename = out_log_path_.path();
	outlog_filename += "/";
	outlog_filename += log_file_name;

	QFile read_table_log(outlog_filename);
	read_table_log.open(QIODevice::ReadWrite);
	if (!read_table_log.isWritable())
	{
		fprintf(stderr, "Read excel file data log file [%s] open fail.",
				outlog_filename.toStdString().c_str());
		return -1;
	}
	std::stringstream sstr_stream;

	//ʲô��Ϊɶ����google pb ��debugstringֱ�������Ϊɶ���Լ�����
	sstr_stream << "Read excel file:" << xls_file_name.toStdString().c_str() << " line count" << line_count
		<< "column count " << col_count << std::endl;
	sstr_stream << "Read table:" << tc_data.excel_table_name_.toStdString().c_str() << std::endl;

	fprintf(stderr, "Read excel file:%s table :%s start. line count %u column %u.",
			xls_file_name.toStdString().c_str(),
			tc_data.excel_table_name_.toStdString().c_str(),
			line_count,
			col_count);

	QString read_data;
	std::string set_data;

	for (int line_no = tc_data.read_data_start_; line_no <= line_count; ++line_no)
	{
		google::protobuf::Message *line_msg = NULL;
		Illusion_Protobuf_Reflect::locate_sub_msg(list_msg,
												  "list_data",
												  true,
												  line_msg);

		google::protobuf::Message *field_msg = NULL;
		const google::protobuf::FieldDescriptor *field_desc = NULL;
		field_msg_ary.clear();
		field_desc_ary.clear();
		for (int col_no = 1; col_no <= col_count; ++col_no)
		{
			//���Ϊ�ձ�ʾ����Ҫ��ע����
			if (tc_data.proto_field_ary_[col_no - 1].length() == 0)
			{
				field_msg_ary.push_back(NULL);
				field_desc_ary.push_back(NULL);
				continue;
			}

			//ȡ���ֶε�����
			ret = Illusion_Protobuf_Reflect::get_fielddesc(line_msg,
														   tc_data.proto_field_ary_[col_no - 1].toStdString(),
														   tc_data.item_msg_firstshow_[col_no - 1] == 1 ? true : false,
														   field_msg,
														   field_desc);
			if (0 != ret)
			{
				fprintf(stderr, "Message [%s] don't find field_desc [%s] field_desc name define in Line/Column[%d/%d(%s)]",
						tc_data.pb_line_message_.toStdString().c_str(),
						tc_data.proto_field_ary_[col_no - 1].toStdString().c_str(),
						tc_data.pb_fieldname_line_,
						col_no,
						BikoQtExcelEngine::column_name(col_no)
				);
				return ret;
			}
			field_msg_ary.push_back(field_msg);
			field_desc_ary.push_back(field_desc);
		}
		fprintf(stderr, "Read line [%d] ", line_no);
		sstr_stream << "Read line:" << line_no << std::endl << "{" << std::endl;

		for (long col_no = 1; col_no <= col_count; ++col_no)
		{
			//���Ϊ�ձ�ʾ����Ҫ��ע����
			if (tc_data.proto_field_ary_[col_no - 1].length() == 0)
			{
				continue;
			}

			//����EXCEL���ݣ�ע������ط��Ǹ���MFC�ı������CString���ݵı���
			read_data = ils_excel_file_.get_cell(line_no, col_no).toString();

			//ȡ���ֶε�����
			field_msg = field_msg_ary[col_no - 1];
			field_desc = field_desc_ary[col_no - 1];

			//�����string ���ͣ�Google PB֧֮��UTF8
			if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_STRING)
			{
				set_data = read_data.toStdString();
			}
			//����BYTES��
			else if (field_desc->type() == google::protobuf::FieldDescriptor::Type::TYPE_BYTES)
			{
				set_data = read_data.toLocal8Bit();
			}
			//�����ֶ�����ͳһת��ΪUTF8�ı���
			else
			{
				set_data = read_data.toStdString();
			}
			//���������������ֶε�����
			ret = Illusion_Protobuf_Reflect::set_fielddata(field_msg, field_desc, set_data);
			if (0 != ret)
			{
				fprintf(stderr, "Message [%s] field_desc [%s] type [%d][%s] set_fielddata fail. Line,Colmn[%d|%d(%s)]",
						tc_data.pb_line_message_.toStdString().c_str(),
						field_desc->full_name().c_str(),
						field_desc->type(),
						field_desc->type_name(),
						line_no,
						col_no,
						BikoQtExcelEngine::column_name(col_no)
				);
				return ret;
			}

			sstr_stream << "\t" << tc_data.proto_field_ary_[col_no - 1].toStdString().c_str() << ":" << set_data.c_str()
				<< std::endl;
		}

		//���û�г�ʼ��
		if (!line_msg->IsInitialized())
		{
			fprintf(stderr, "Read line [%d] message [%s] is not IsInitialized, please check your excel or proto file.",
					line_no,
					tc_data.pb_line_message_.toStdString().c_str());

			fprintf(stderr, "Read line [%d] message [%s] InitializationErrorString :%s;",
					line_no,
					tc_data.pb_line_message_.toStdString().c_str(),
					line_msg->InitializationErrorString().c_str());
			return -1;
		}

		std::cout << line_msg->DebugString() << std::endl;
	}

	std::string out_string;
	out_string.reserve(64 * 1024 * 1024);
	out_string = sstr_stream.str();

	fprintf(stderr, "\n%s", out_string.c_str());
	read_table_log.write(out_string.c_str(), out_string.length());
	read_table_log.close();
	fprintf(stderr, "Read excel file:%s table :%s end.",
			xls_file_name.toStdString().c_str(),
			tc_data.excel_table_name_.toStdString().c_str());
	return 0;
}



int Biko_Read_Config::save_to_protocfg(const TABLE_CONFIG & table_cfg, 
										   const google::protobuf::Message * line_msg, 
										   QString & error_tips)
{
	QString pbc_file = out_pbc_path_.path() + "/";
	pbc_file += table_cfg.save_pb_config_;
	QString txt_file = pbc_file + ".txt";
	QFile pbc_config(pbc_file);
	pbc_config.open(QIODevice::ReadWrite);
	if (!pbc_config.isWritable())
	{
		return -1;
	}
	QFile txt_config(txt_file);
	txt_config.open(QIODevice::ReadWrite);
	if (!txt_config.isWritable())
	{
		return -1;
	}
	if (!line_msg->IsInitialized())
	{
		fprintf(stderr, "class [%s] protobuf encode fail, IsInitialized return false.error string [%s].",
				line_msg->GetTypeName().c_str(),
				line_msg->InitializationErrorString().c_str());
		return -1;
	}
	std::string bin_string, txt_string;
	bool succ = line_msg->SerializeToString(&bin_string);
	if (!succ)
	{
		return -2;
	}
	succ = google::protobuf::TextFormat::PrintToString(*line_msg, &txt_string);
	if (!succ)
	{
		return -2;
	}
	qint64 wt_len = pbc_config.write(bin_string.c_str(), bin_string.length());
	if (wt_len < 0)
	{
		return -3;
	}
	pbc_config.close();

	wt_len = txt_config.write(txt_string.c_str(), txt_string.length());
	if (wt_len < 0)
	{
		return -4;
	}
	txt_config.close();
	return 0;
}


