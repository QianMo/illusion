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

void ZCE_Error_Collector::toQStringList(QStringList & tips_ary)
{
	for (size_t i = 0; i < error_array_.size(); ++i)
	{
		tips_ary.append(QString::fromLocal8Bit("file[%1]line[%2]column[%3]error [%4]").
						arg(error_array_[i].file_name_.c_str()).
						arg(error_array_[i].line_).
						arg(error_array_[i].column_).
						arg(error_array_[i].message_.c_str()));
	}
}


//======================================================================================
//����ĵ���ʵ��
Biko_Read_Config *Biko_Read_Config::instance_ = NULL;

//
Biko_Read_Config::Biko_Read_Config()
{
	source_tree_ = new  google::protobuf::compiler::DiskSourceTree();
	protobuf_importer_ = new google::protobuf::compiler::Importer(source_tree_, &error_collector_);
	msg_factory_ = new google::protobuf::DynamicMessageFactory();
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
                                     QStringList &tips_ary)
{
	QStringList import_list;
	import_list.append(allinone_dir + "/import");
    return init_read_all(allinone_dir + "/proto",
						 allinone_dir + "/excel",
                         allinone_dir + "/outer",
						 import_list,
                         tips_ary);
}

//��ȡexcel_dirĿ¼�����е�EXCEL�ļ�������proto_dirĿ¼�µ�meta�ļ������䣬ת����λ���ļ������outer_dirĿ¼
int Biko_Read_Config::init_read_all(const QString &proto_dir,
                                    const QString &excel_dir,
                                    const QString &outer_dir,
								    const QStringList &import_list,
                                    QStringList &tips_ary)
{

    int ret = 0;

	init_importdir(import_list, tips_ary);
	
    //�������ֶ����г�ʼ������
    ret = init_protodir(proto_dir, tips_ary);
    if (ret != 0)
    {
        return -1;
    }
    ret = init_exceldir(excel_dir, tips_ary);
    if (ret != 0)
    {
        return -1;
    }

    ret = init_outdir(outer_dir, tips_ary);
    if (ret != 0)
    {
        return -1;
    }

    bool bret = ils_excel_file_.initialize(false);
    if (false == bret)
    {
        tips_ary.append(QString::fromLocal8Bit("OLE��������EXCEL��ʵ��OLE��ȡEXCEL���밲װ��EXCEL��"));
        return -1;
    }

    return 0;
}

void Biko_Read_Config::init_importdir(const QStringList &import_list,
					                  QStringList & /*tips_ary*/)
{
	for (int i = 0; i < import_list.size(); ++i)
	{
		source_tree_->MapPath("", import_list[i].toStdString());
	}
	
}

//��ʼ��.proto�ļ�Ŀ¼����ȡ�������е�proto�ļ�
int Biko_Read_Config::init_protodir(const QString &proto_dir,
                                    QStringList &tips_ary)
{
    int ret = 0;
    proto_path_.setPath(proto_dir);
    if (false == proto_path_.exists())
    {
        tips_ary.append(QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
                        arg(proto_dir));
        return -1;
    }
    //��ȡ.proto �ļ�
    QStringList filters;
    filters << "*.proto";
    proto_fileary_ = proto_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
    if (proto_fileary_.size() <= 0)
    {
        tips_ary.append(QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�protobuf meta(.proto)�ļ������������").
                        arg(proto_dir));
        return -1;
    }
    
    source_tree_->MapPath("", proto_path_.path().toStdString());

    //�������е�.proto �ļ�
    for (int i = 0; i < proto_fileary_.size(); ++i)
    {
        ret = read_proto_file(proto_fileary_[i], tips_ary);
        if (0 != ret)
        {
            return -1;
        }
    }
    return 0;
}

//
int Biko_Read_Config::init_exceldir(const QString &excel_dir,
                                    QStringList &tips_ary)
{
    int ret = 0;
    excel_path_.setPath(excel_dir);
    if (false == excel_path_.exists())
    {
        tips_ary.append(QString::fromLocal8Bit("Ŀ¼[%1]�������ڣ����������").
                        arg(excel_dir));
        return -1;
    }
    //��ȡ.xls , .xlsx �ļ�
    QStringList filters;
    filters << "*.xls" << "*.xlsx";
    excel_fileary_ = excel_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
    if (excel_fileary_.size() <= 0)
    {
        tips_ary.append(QString::fromLocal8Bit("Ŀ¼[%1]��û���κ�Excel�ļ������������").
                        arg(excel_dir));
        return -1;
    }
    return 0;
}

int Biko_Read_Config::init_outdir(const QString &outer_dir,
                                  QStringList &tips_ary)
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

    if (ils_excel_file_.isOpen())
    {
        ils_excel_file_.close();
        ils_excel_file_.finalize();
    }

    return;
}

//�������еĶ�ȡ����
void Biko_Read_Config::clear()
{
	msgname_2_illusion_map_.clear();
    excel_2_illusion_map_.clear();
    proto_2_illusion_map_.clear();
    outer_2_illusion_map_.clear();
	//
	for (const Illusion_Message *&ils_msg: illusion_msg_ary_)
	{
		delete ils_msg;
		ils_msg = NULL;
	}
	illusion_msg_ary_.clear();
}

//��ɨ����߲�����EXCEL�ļ�������һ�ζ�ȡ
int Biko_Read_Config::read_all_message(QStringList &tips_ary)
{
    int ret = 0;
    for (auto iter = excel_2_illusion_map_.begin(); iter != excel_2_illusion_map_.end(); ++iter)
    {
        //��EXCEL�ļ�
        ret = open_excel_file(iter->first,
                              false,
                              tips_ary);
        if (0 != ret)
        {
            return ret;
        }

        //��ȡÿһ��Message
        for (size_t i = 0; i < iter->second.size(); ++i)
        {
            ret = read_excel_table(iter->second[i], tips_ary);
			if (0 != ret)
			{
				close_excel_file();
				return ret;
			}
        }

        //�ر�EXCEL �ļ�
        close_excel_file();
    }
    return 0;
}


int Biko_Read_Config::read_one_message(const QString &messge_full_name,
                                       QStringList &tips_ary)
{
    int ret = 0;
    auto iter = msgname_2_illusion_map_.find(messge_full_name);
    if (iter == msgname_2_illusion_map_.end())
    {
        tips_ary.append(QString::fromLocal8Bit("[%1] Message �����޷��ҵ������������������.proto�ļ���")
                        .arg(messge_full_name));
        return -1;
    }

    ret = open_excel_file(iter->second->excel_file_name_,
                          false,
                          tips_ary);
    if (0 != ret)
    {
        return ret;
    }
	ret = read_excel_table(iter->second, tips_ary);
	if (0 != ret)
	{
		close_excel_file();
		return ret;
	}

	//�ر�EXCEL �ļ�
	close_excel_file();

    return 0;
}


//
int Biko_Read_Config::read_proto_file(const QFileInfo &proto_file,
                                      QStringList &tips_ary)
{
    int ret = 0;
    const google::protobuf::FileDescriptor *file_desc = NULL;

    error_collector_.clear_error();
    QString proto_fname;
    proto_fname = proto_file.fileName();

    file_desc = protobuf_importer_->Import(proto_fname.toStdString());
    if (!file_desc)
    {
        fprintf(stdout, "Importer Import filename [%s] fail.\n",
				proto_fname.toStdString().c_str());
		error_collector_.toQStringList(tips_ary);
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
            if (false == mo.GetExtension(illusion::cfg_message))
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


            auto iter1 = proto_2_illusion_map_.find(proto_fname);
            if (iter1 == proto_2_illusion_map_.end())
            {
                std::vector<const Illusion_Message *> ils_msg_ary;
                ils_msg_ary.push_back(ok_ptr);
                proto_2_illusion_map_[proto_fname] = ils_msg_ary;
            }
            else
            {
                iter1->second.push_back(ok_ptr);
            }

            QString excel_fname = ok_ptr->excel_file_name_;
            auto iter2 = excel_2_illusion_map_.find(excel_fname);
            if (iter2 == excel_2_illusion_map_.end())
            {
                std::vector<const Illusion_Message *> ils_msg_ary;
                ils_msg_ary.push_back(ok_ptr);
                excel_2_illusion_map_[excel_fname] = ils_msg_ary;
            }
            else
            {
                iter2->second.push_back(ok_ptr);
            }

            QString outer_fname = ok_ptr->outer_file_name_;
            auto iter3 = outer_2_illusion_map_.find(outer_fname);
            if (iter3 == outer_2_illusion_map_.end())
            {
                outer_2_illusion_map_[outer_fname] = ok_ptr;
            }

        }
    }
    return 0;
}




//��ȡ���е�ö��ֵ
int Biko_Read_Config::read_table_enum(MAP_QSTRING_TO_QSTRING &enum_map,
									  QStringList &tips_ary)
{

    //���Ƿ���loead ���sheet
    bool bret =  ils_excel_file_.loadSheet("ENUM_CONFIG");
    if (bret == false)
    {
		tips_ary.append(QString::fromLocal8Bit("��ѡ�������EXCEL�����ܶ�ȡ�����ñ�[ENUM_CONFIG]."
											   "û��ö��ֵ��Ҫ��ȡ��"));
        return -1;
    }

    //��Ӧ����
    int row_count = ils_excel_file_.rowCount();
    int col_count = ils_excel_file_.columnCount();
	fprintf(stdout, "ENUM_CONFIG table have col_count = %d and row_count =%d. \n",
			col_count,
			row_count);

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
        QString enum_key = ils_excel_file_.getCell(row_no, COL_ENUM_KEY).toString();

        //�����һ���ַ���[
        if (enum_key[0] == ENUM_FIRST_CHAR )
        {
            QString enum_vlaue = ils_excel_file_.getCell(row_no, COL_ENUM_VALUE).toString();
            enum_map[enum_key] = enum_vlaue;

            ++read_enum;
        }
    }
    return 0;
}


//!
int Biko_Read_Config::open_excel_file(const QString &excel_file_name,
                                      bool not_exist_new,
                                      QStringList &tips_ary)
{
    QString excel_path = excel_path_.absolutePath() +"/" + excel_file_name;
    bool bret = ils_excel_file_.open(excel_path, not_exist_new);
    //Excel�ļ���ʧ��
    if (bret != true)
    {
        return -1;
    }
    fprintf(stderr, "Dream excecl file have sheet num[%d].\n",
            ils_excel_file_.sheetsCount());
    return 0;
}

void Biko_Read_Config::close_excel_file()
{
	ils_excel_file_.close();
}

//
int Biko_Read_Config::read_excel_table(const Illusion_Message *ils_msg,
                                       QStringList &tips_ary)
{
    int ret = 0;
    //���EXCEL�ļ����Ƿ���������
    if (ils_excel_file_.loadSheet(ils_msg->excel_sheet_name_) == false)
    {
        tips_ary.append(QString::fromLocal8Bit("!��ѡ�������EXCEL�ļ�[%1]�е�"
                                               "[%2]�����ڻ��߲���ȷ�������ּ���򿪡�!")
                        .arg(ils_msg->excel_file_name_)
                        .arg(ils_msg->excel_sheet_name_));
        return -3;
    }


    int line_count = ils_excel_file_.rowCount();
    int col_count = ils_excel_file_.columnCount();
    fprintf(stderr, "Read excel table %s table have col_count %d row_count %d.\n",
            ils_msg->excel_sheet_name_.toStdString().c_str(),
            col_count,
            line_count);


    //�����ʶ��pb�ֶ��еȣ�����ʵû����ô����
    if (ils_msg->read_data_line_ > line_count ||
        ils_msg->fieldsname_line_ > line_count ||
        ils_msg->column_field_count_ > col_count)
    {
        tips_ary.append(QString::fromLocal8Bit("��ѡ�������EXCEL[%1]�ļ��е�"
                                               "[%2]��û���㹻�������Թ���ȡ��!").
                        arg(ils_msg->excel_file_name_).
                        arg(ils_msg->excel_sheet_name_));
        return -4;
    }

    fprintf(stderr, "Read excel file:%s table :%s start. line count %u column %u.\n",
            ils_msg->excel_file_name_.toStdString().c_str(),
            ils_msg->excel_sheet_name_.toStdString().c_str(),
            line_count,
            col_count);



    //�������������ֶε���ʼ��#,%,!����Щ�ֶβ���ȡ������
    std::vector<int> read_col;
    if (col_count == ils_msg->column_field_count_)
    {
        for (int col_no = 1; col_no <= col_count; ++col_no)
        {
            read_col.push_back(col_no);
        }
    }
    else if (col_count > ils_msg->column_field_count_)
    {
        for (int col_no = 1;
             col_no <= col_count && static_cast<int>(read_col.size()) < ils_msg->column_field_count_; ++col_no)
        {
            QVariant data = ils_excel_file_.getCell(ils_msg->fieldsname_line_,
                                                    col_no);
            QString field_name = data.toString();
            QChar first_char = field_name.at(0);
            if (first_char == '#' || first_char == '%' ||  first_char == '!')
            {
                continue;
            }
            else
            {
                read_col.push_back(col_no);
            }
        }
        if (read_col.size() != ils_msg->column_field_count_)
        {
            return -1;
        }
    }
    else
    {
        //�����ܵ���
        Q_ASSERT(false);
    }

    google::protobuf::Message *table_msg = NULL;
    ret = ils_msg->new_table_mesage(msg_factory_,
                                    table_msg);
    if (0 != ret || NULL == table_msg )
    {
        return -2;
    }
    std::unique_ptr<google::protobuf::Message> ptr_msg(table_msg);

    for (int line_no = ils_msg->read_data_line_; line_no <= line_count; ++line_no)
    {
        std::vector<QString > line_data_ary;
        line_data_ary.reserve(col_count);
        fprintf(stderr, "Read line [%d] \n", line_no);
        for (int col_no = 1; col_no <= col_count; ++col_no)
        {
            QString cell_data = ils_excel_file_.getCell(line_no,
                                                            read_col[col_no-1]).toString();
            line_data_ary.push_back(cell_data);

        }
        int error_field_no = -1;
        const google::protobuf::FieldDescriptor *error_field_desc = NULL;
        ret = ils_msg->add_line(table_msg,
                                line_data_ary,
                                error_field_no,
                                error_field_desc);
        if (0 != ret)
        {
            QString error_info = QString::fromLocal8Bit("!��̬д��Messageʧ�ܣ�Table Message [%1] Line Message [%2] �ֶ�����[%3]."
                                                        "��Ӧ�ֶ��ǵ�EXCEL �ļ�[%4]Sheet ����[%5].�к��к�[%6|%7]%8")
                                 .arg(ils_msg->table_messge_name_)
                                 .arg(ils_msg->line_message_name_)
                                 .arg(error_field_desc->full_name().c_str())
                                 .arg(ils_msg->excel_file_name_)
                                 .arg(ils_msg->excel_sheet_name_)
                                 .arg(line_no)
                                 .arg(read_col[error_field_no])
                                 .arg(QtAxExcelEngine::columnName(read_col[error_field_no]));
            fprintf(stderr, "%s\n", error_info.toStdString().c_str());
            tips_ary.append(error_info);
            return ret;
        }
    }
    //���û�г�ʼ��
    if (!table_msg->IsInitialized())
    {
        fprintf(stderr, "Read table message [%s] is not IsInitialized, please check your excel or proto file.\n",
                ils_msg->table_messge_name_.toStdString().c_str());
        return -1;
    }
    std::cout << table_msg->DebugString() << std::endl;
    fprintf(stderr, "Read excel file:%s table :%s end.\n",
            ils_msg->excel_file_name_.toStdString().c_str(),
            ils_msg->excel_sheet_name_.toStdString().c_str());


    ret = save_to_protocfg(ils_msg, table_msg, tips_ary);
    if (0 != ret)
    {
        return ret;
    }
    return 0;
}

//
int Biko_Read_Config::save_excel_tablehead(const QString &messge_full_name,
                                           QStringList &tips_ary)
{
    int ret = 0;

	auto iter = msgname_2_illusion_map_.find(messge_full_name);
	if (iter == msgname_2_illusion_map_.end())
	{
		return -1;
	}
	const Illusion_Message *ils_msg = iter->second;

	//��EXCEL�ļ�
	ret = open_excel_file(ils_msg->excel_file_name_,
						  true,
						  tips_ary);
	if (0 != ret)
	{
		return ret;
	}

    //���EXCEL�ļ����Ƿ���������
    if (ils_excel_file_.loadSheet(ils_msg->excel_sheet_name_) == true)
    {
        QString bak_name = ils_msg->excel_sheet_name_;
		bak_name += ".BAK";
        ils_excel_file_.renameSheet(bak_name);
    }
	
    ils_excel_file_.insertSheet(ils_msg->excel_sheet_name_);

    for (int i = 0; i < ils_msg->column_field_count_; ++i)
    {
        ils_excel_file_.setCell(ils_msg->fieldsname_line_,
                                i + 1,
                                ils_msg->column_fieldname_ary_[i]);
        if (ils_msg->fullname_line_ > 0)
        {
            ils_excel_file_.setCell(ils_msg->fullname_line_,
                                    i + 1,
                                    ils_msg->column_fullname_ary_[i]);
        }
    }
	
	ils_excel_file_.save();

	close_excel_file();

    return 0;
}

int Biko_Read_Config::save_to_protocfg(const Illusion_Message *ils_msg,
                                       const google::protobuf::Message *table_msg,
                                       QStringList &tips_ary) const
{
    QString pbc_file = out_pbc_path_.path() + "/";
    pbc_file += ils_msg->outer_file_name_;
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
    if (!table_msg->IsInitialized())
    {
        fprintf(stderr, "class [%s] protobuf encode fail, IsInitialized return false.error string [%s].",
                table_msg->GetTypeName().c_str(),
                table_msg->InitializationErrorString().c_str());
        return -1;
    }
    std::string bin_string, txt_string;
    bool succ = table_msg->SerializeToString(&bin_string);
    if (!succ)
    {
        return -2;
    }
    succ = google::protobuf::TextFormat::PrintToString(*table_msg, &txt_string);
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





