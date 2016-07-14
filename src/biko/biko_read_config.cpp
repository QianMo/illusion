#include "biko_predefine.h"
#include "biko_protobuf_reflect.h"
#include "biko_read_config.h"

//======================================================================================
//�����Ĵ�����Ϣ�ռ�װ�ã�
ZCE_Error_Collector::ZCE_Error_Collector()
{
}


ZCE_Error_Collector::~ZCE_Error_Collector()
{
}

void ZCE_Error_Collector::AddError(const std::string &file_name,
                                   int line,
                                   int column,
                                   const std::string &message)
{
    ZCE_PROTO_ERROR proto_err;
    proto_err.file_name_ = file_name;
    proto_err.line_ = line;
    proto_err.column_ = column;
    proto_err.message_ = message;

    error_array_.push_back(proto_err);
}

void ZCE_Error_Collector::clear_error()
{
    error_array_.clear();
}


//======================================================================================
//����ĵ���ʵ��
Biko_Read_Config *Biko_Read_Config::instance_ = NULL;

//
Biko_Read_Config::Biko_Read_Config()
{
}


Biko_Read_Config::~Biko_Read_Config()
{
    //��3������������˳��ģ�ע��
    if (msg_factory_)
    {
        delete msg_factory_;
        msg_factory_ = NULL;
    }
    if (source_tree_)
    {
        delete source_tree_;
        source_tree_ = NULL;
    }
    if (protobuf_importer_)
    {
        delete protobuf_importer_;
        protobuf_importer_ = NULL;
    }
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
                                     QStringList &error_tips)
{
    return init_read_all(allinone_dir + "/excel",
                         allinone_dir + "/proto",
						 allinone_dir + "/outer",
                         error_tips);
}

//��ȡexcel_dirĿ¼�����е�EXCEL�ļ�������proto_dirĿ¼�µ�meta�ļ������䣬ת����λ���ļ������outer_dirĿ¼
int Biko_Read_Config::init_read_all(const QString &proto_dir,
                                    const QString &excel_dir,
                                    const QString &outer_dir,
                                    QStringList &error_tips)
{
    
	int ret = 0;
	//�������ֶ����г�ʼ������
	ret = init_protodir(proto_dir, error_tips);
	if (ret != 0)
	{
		return -1;
	}
	ret = init_exceldir(excel_dir, error_tips);
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
		error_tips.append(QString::fromLocal8Bit("OLE��������EXCEL��ʵ��OLE��ȡEXCEL���밲װ��EXCEL��"));
		return -1;
	}

	return 0;
}

//��ʼ��.proto�ļ�Ŀ¼����ȡ�������е�proto�ļ�
int Biko_Read_Config::init_protodir(const QString &proto_dir,
                                    QStringList &error_tips)
{
    int ret = 0;
    proto_path_.setPath(proto_dir);;
    if (false == proto_path_.exists())
    {
        error_tips.append(QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
                          arg(proto_dir));
        return -1;
    }
    //��ȡ.proto �ļ�
    QStringList filters;
    filters << "*.proto";
    proto_fileary_ = proto_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
    if (proto_fileary_.size() <= 0)
    {
        error_tips.append(QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�protobuf meta(.proto)�ļ������������").
                          arg(proto_dir));
        return -1;
    }

    source_tree_ = new  google::protobuf::compiler::DiskSourceTree();
    source_tree_->MapPath("", proto_path_.path().toStdString());
    protobuf_importer_ = new google::protobuf::compiler::Importer(source_tree_, &error_collector_);
    msg_factory_ = new google::protobuf::DynamicMessageFactory();

    //�������е�.proto �ļ�
    for (int i = 0; i < proto_fileary_.size(); ++i)
    {
        ret = read_proto_file(proto_fileary_[i], error_tips);
        if (0 != ret)
        {
            return -1;
        }
    }
    return 0;
}

//
int Biko_Read_Config::init_exceldir(const QString & excel_dir,
									QStringList & error_tips)
{
	int ret = 0;
	excel_path_.setPath(excel_dir);
	if (false == excel_path_.exists())
	{
		error_tips.append(QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
						  arg(excel_dir));
		return -1;
	}
	//��ȡ.xls , .xlsx �ļ�
	QStringList filters;
	filters << "*.xls" << "*.xlsx";
	excel_fileary_ = excel_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
	if (excel_fileary_.size() <= 0)
	{
		error_tips.append(QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�Excel�ļ������������").
						  arg(excel_dir));
		return -1;
	}
	return 0;
}

int Biko_Read_Config::init_outdir(const QString &outer_dir,
                                  QStringList &error_tips)
{
    QString path_str;

    path_str = outer_dir;
    path_str += "/log";
    out_log_path_.setPath(path_str);
    if (false == out_log_path_.exists())
    {
        if (false == out_log_path_.mkpath(path_str))
        {
            return -1;
        }
    }

    //pbc��·��û�п��Դ���
    path_str = outer_dir;
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
    excel_cfg_map_.clear();
    proto_cfg_map_.clear();
    outer_cfg_map_.clear();

}

//��ɨ����߲�����EXCEL�ļ�������һ�ζ�ȡ
int Biko_Read_Config::read_all_message(QStringList &error_tips)
{
	int ret = 0;
	for (size_t i = 0; i < illusion_msg_ary_.size(); ++i)
	{
		ret = open_illusion_excel(illusion_msg_ary_[i]->excel_file_name_,
								  false,
								  error_tips);
		if (0 != ret)
		{
			return ret;
		}
	}
    return 0;
}

int Biko_Read_Config::read_one_message(const QString & messge_full_name, 
									   QStringList & error_tips)
{
	int ret = 0;
	auto iter = msgname_2_illusion_map_.find(messge_full_name);
	if (iter == msgname_2_illusion_map_.end())
	{
		error_tips.append(QString::fromLocal8Bit("[%1] Message �����޷��ҵ������������������.proto�ļ���")
						  .arg(messge_full_name));
		return -1;
	}

	ret = open_illusion_excel(iter->second->excel_file_name_,
							  false,
							  error_tips);
	if (0 != ret)
	{
		return ret;
	}
	return 0;
}




//
int Biko_Read_Config::read_proto_file(const QFileInfo &proto_file,
                                      QStringList &tips_info)
{
    int ret = 0;
    const google::protobuf::FileDescriptor *file_desc = NULL;

    error_collector_.clear_error();
    QString proto_path;
    QString proto_fname;
    proto_path = proto_file.path();
    proto_fname = proto_file.fileName();

    file_desc = protobuf_importer_->Import(proto_path.toStdString());
    if (!file_desc)
    {
        fprintf(stderr, "Importer Import filename [%s] fail.",
                proto_path.toStdString().c_str());
        return -1;
    }

    //�������е�Message���ҳ�����Ҫ����ģ�
    int msg_count = file_desc->message_type_count();
    for (int i = 0; i < msg_count; ++i)
    {
        const google::protobuf::Descriptor *table_msg_desc = file_desc->message_type(i);

        if (table_msg_desc && table_msg_desc->field_count() > 0 )
        {
            const google::protobuf::MessageOptions &mo = table_msg_desc->options();
            if (false == mo.GetExtension(illusion::illusion_message))
            {
                continue;
            }
            std::unique_ptr<Illusion_Message> ils_ptr(new Illusion_Message());
            ret = ils_ptr->init(table_msg_desc);
            if (0 != ret)
            {
                return ret;
            }

            Illusion_Message *ok_ptr = ils_ptr.get();
            ils_ptr.release();

			illusion_msg_ary_.push_back(ok_ptr);

			QString msg_name = ok_ptr->table_messge_name_;
			msgname_2_illusion_map_[msg_name] = ok_ptr;
			

            auto iter1 = proto_cfg_map_.find(proto_fname);
            if (iter1 == proto_cfg_map_.end())
            {
                std::vector<const Illusion_Message *> ils_msg_ary;
                ils_msg_ary.push_back(ok_ptr);
                proto_cfg_map_[proto_fname] = ils_msg_ary;
            }
            else
            {
				iter1->second.push_back(ok_ptr);
            }

			QString excel_fname = ok_ptr->excel_file_name_;
			auto iter2 = excel_cfg_map_.find(excel_fname);
			if (iter2 == excel_cfg_map_.end())
			{
				std::vector<const Illusion_Message *> ils_msg_ary;
				ils_msg_ary.push_back(ok_ptr);
				excel_cfg_map_[excel_fname] = ils_msg_ary;
			}
			else
			{
				iter2->second.push_back(ok_ptr);
			}

			QString outer_fname = ok_ptr->outer_file_name_;
			auto iter3 = outer_cfg_map_.find(outer_fname);
			if (iter3 == outer_cfg_map_.end())
			{
				outer_cfg_map_[outer_fname] = ok_ptr;
			}

        }
    }
    return 0;
}




//��ȡ���е�ö��ֵ
int Biko_Read_Config::read_table_enum(MAP_QSTRING_TO_QSTRING &enum_map)
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
            enum_map[enum_key] = enum_vlaue;

            ++read_enum;
        }
    }
    return 0;
}


//!
int Biko_Read_Config::open_illusion_excel(const QString &excel_file_name,
										  bool read_enum_sheet,
										  QStringList &error_tips)
{
	QString excel_path = excel_path_.path() + excel_file_name;
	bool bret = ils_excel_file_.open(excel_path);
	//Excel�ļ���ʧ��
	if (bret != true)
	{
		return -1;
	}
	fprintf(stderr, "Dream excecl file have sheet num[%d].\n",
			ils_excel_file_.sheetsCount());

	//������
	MAP_QSTRING_TO_QSTRING enum_map;
	if (read_enum_sheet)
	{
		if (ils_excel_file_.hasSheet("ENUM_CONFIG") == false)
		{
			error_tips.append(QString::fromLocal8Bit("��ѡ�������EXCEL�����ܶ�ȡ�����ñ�[ENUM_CONFIG]."
													 "û��ö��ֵ��Ҫ��ȡ��"));
		}
		else
		{
			//
			int ret = read_table_enum(enum_map);
			if (0 != ret)
			{
				error_tips.append(QString::fromLocal8Bit("��ѡ�������EXCEL�ļ��е�"
														 "[ENUM_CONFIG]����ȷ�������ּ���򿪡�!"));
				return ret;
			}
		}
	}

	return 0;
}

//!
int Biko_Read_Config::read_illusion_excel(const Illusion_Message *ils_msg,
										  QStringList &error_tips)
{
	int ret = 0;
	
	QString excel_file = ils_msg->excel_file_name_;
	QString sheet_name = ils_msg->excel_sheetname_;
	QString table_message = ils_msg->table_messge_name_;
	QString line_message = ils_msg->line_messge_name_;

	//���EXCEL�ļ����Ƿ���������
	if (ils_excel_file_.loadSheet(sheet_name) == false)
	{
		error_tips.append(QString::fromLocal8Bit("��ѡ�������EXCEL�ļ�[%1]�е�"
												 "[%2]�����ڻ��߲���ȷ�������ּ���򿪡�!")
						  .arg(excel_file)
						  .arg(sheet_name));
	    return -3;
	}
	
	
	int line_count = ils_excel_file_.row_count();
	int col_count = ils_excel_file_.column_count();
	fprintf(stderr, "Read excel table %s table have col_count %d row_count %d.\n",
			sheet_name.toStdString().c_str(),
			col_count,
			line_count);
	    

	//�����ʶ��pb�ֶ��еȣ�����ʵû����ô����
	if (ils_msg->read_data_line_ > line_count ||
		ils_msg->fieldsname_line_ > line_count ||
		ils_msg->tb_field_count_ > col_count)
	{
		error_tips.append(QString::fromLocal8Bit("��ѡ�������EXCEL[%1]�ļ��е�"
												 "[%2]��û���㹻�������Թ���ȡ��!")
						  .arg(excel_file)
						  .arg(sheet_name));
	    return -4;
	}
	
	fprintf(stderr, "Read excel file:%s table :%s start. line count %u column %u.",
			excel_file.toStdString().c_str(),
			sheet_name.toStdString().c_str(),
	        line_count,
	        col_count);
	
	google::protobuf::Message *table_msg = NULL;
	ils_msg->new_table_mesage(msg_factory_,
							  table_msg);
	
	int readcol_num = 0;

	if (col_count > ils_msg->tb_field_count_)
	{
		for (int col_no = 1; col_no <= col_count; ++col_no)
		{
			if (readcol_num >= )
		}
	}
	

	for (int line_no = ils_msg->read_data_line_; line_no <= line_count; ++line_no)
	{

	    fprintf(stderr, "Read line [%d] ", line_no);
	    for (int col_no = 1; col_no <= col_count; ++col_no)
	    {

	    }
	
		//fprintf(stderr, "Message [%s] field_desc [%s] type [%d][%s] set_fielddata fail. Line,Colmn[%d|%d(%s)]",
		//        tc_data.pb_line_message_.toStdString().c_str(),
		//        field_desc->full_name().c_str(),
		//        field_desc->type(),
		//        field_desc->type_name(),
		//        line_no,
		//        col_no,
		//        BikoQtExcelEngine::column_name(col_no)
		//);
	
	    
	}
	//���û�г�ʼ��
	if (!table_msg->IsInitialized())
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
	fprintf(stderr, "Read excel file:%s table :%s end.",
			excel_file.toStdString().c_str(),
			sheet_name.toStdString().c_str());
	return 0;
}



