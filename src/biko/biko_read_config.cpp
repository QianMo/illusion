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

void ZCE_Error_Collector::to_qstringlist(QStringList &tips_ary)
{
    for (size_t i = 0; i < error_array_.size(); ++i)
    {
        tips_ary.append(QString::fromLocal8Bit("File[%1]line[%2]column[%3]error [%4]").
                        arg(error_array_[i].file_name_.c_str()).
                        arg(error_array_[i].line_).
                        arg(error_array_[i].column_).
                        arg(error_array_[i].message_.c_str()));
    }
}

void ZCE_Error_Collector::print_tostdout()
{
    for (size_t i = 0; i < error_array_.size(); ++i)
    {
        fprintf(stdout,
                "File[%s]line[%d]column[%d] have error [%s] \n",
                error_array_[i].file_name_.c_str(),
                error_array_[i].line_,
                error_array_[i].column_,
                error_array_[i].message_.c_str());
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

//��ʼ����ȡ������Ŀ¼��Ϊ������������
int Biko_Read_Config::init_read_all(const QString &proto_dir,
                                    const QString &excel_dir,
                                    const QString &outer_dir,
                                    const QStringList &import_list,
                                    QStringList &tips_ary)
{

    int ret = 0;
    QString tip_info;
    //�Բ��ɼ���ʽ����excel����
    clear();
    bool bret = ils_excel_file_.initialize(false);
    if (false == bret)
    {
        tip_info = QString::fromLocal8Bit("OLE��������EXCEL��ʵ��OLE��ȡEXCEL���밲װ��EXCEL��");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("OLE ��һ�����������Ƶ�����������̵ĵ䷶��������ʲô���ⶼ�п��ܡ�");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("1.���EXCEL���ǰ�װ�汾����Ҫָ��ʲô��ɫ�汾��");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("2.�㰲װ�ˣ�����WPS��ţ�ƴ󷢵�WPS�ܶ�ʱ����ע�����ע���EXCELһ���ļ�ֵ��");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("������:");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("1.ж�����е�EXCEL��WPS��ɾ��������ص�Ŀ¼");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("2.����ע����ù�������Wise Registry Cleaner and ��");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("3.��װOffice and EXCEL");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("4.�����Ҿ��ܰ��㵽����ˡ��������и�����������4�����鶼��������1̨OK�ˣ�1̨�������⡣");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit("5.�������ı䣬������libxl ���д�����沿�ִ��롣��");
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }

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
        return -2;
    }

    ret = init_outdir(outer_dir, tips_ary);
    if (ret != 0)
    {
        return -3;
    }

    return 0;
}

//import Ŀ¼
void Biko_Read_Config::init_importdir(const QStringList &import_list,
                                      QStringList & /*tips_ary*/)
{
    for (int i = 0; i < import_list.size(); ++i)
    {
        source_tree_->MapPath("", import_list[i].toLocal8Bit().toStdString());
    }
}

//��ʼ��.proto�ļ�Ŀ¼����ȡ�������е�proto�ļ�
int Biko_Read_Config::init_protodir(const QString &proto_dir,
                                    QStringList &tips_ary)
{
    int ret = 0;
    QString tip_info;
    proto_path_.setPath(proto_dir);
    if (false == proto_path_.exists())
    {
        tip_info = QString::fromLocal8Bit("ProtoĿ¼[%1]�������ڣ����������")
                   .arg(proto_dir);
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }
    //��ȡ.proto �ļ�
    QStringList filters;
    filters << "*.proto";
    proto_fileary_ = proto_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
    if (proto_fileary_.size() <= 0)
    {
        tip_info = QString::fromLocal8Bit("ProtoĿ¼[%1]��û���κ�protobuf meta(.proto)�ļ������������").
                   arg(proto_dir);
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }

    source_tree_->MapPath("", proto_path_.path().toLocal8Bit().toStdString());

    //�������е�.proto �ļ�
    for (int i = 0; i < proto_fileary_.size(); ++i)
    {
        //����Ŀ¼�����ÿһ��.proto file
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
    QString tip_info;
    excel_path_.setPath(excel_dir);
    if (false == excel_path_.exists())
    {
        tip_info = QString::fromLocal8Bit("EXCELĿ¼[%1]�������ڣ����������").
                   arg(excel_dir);
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }
    //��ȡ.xls , .xlsx �ļ�
    QStringList filters;
    filters << "*.xls" << "*.xlsx";
    excel_fileary_ = excel_path_.entryInfoList(filters, QDir::Files | QDir::Readable);
    if (excel_fileary_.size() <= 0)
    {
        tips_ary.append(QString::fromLocal8Bit("EXCELĿ¼[%1]��û���κ�Excel�ļ�(.xls or .xlsx)��"
                                               "������Ǵ���EXCEL�����������").
                        arg(excel_dir));
        illusion::process_tips(tip_info, tips_ary);
    }
    return 0;
}

int Biko_Read_Config::init_outdir(const QString &outer_dir,
                                  QStringList &tips_ary)
{
    QString path_str;
    QString tip_info;
    //pbc��·��û�п��Դ���
    path_str = outer_dir;
    outer_path_.setPath(path_str);
    if (false == outer_path_.exists())
    {
        if (false == outer_path_.mkpath(path_str))
        {
            tip_info = QString::fromLocal8Bit("OUTERĿ¼[%1]�����ڣ����Ҳ��ܴ������Ŀ¼�����������").
                       arg(outer_dir);
            illusion::process_tips(tip_info, tips_ary);
            return -1;
        }
    }

    //�Ƚ��ļ�˭������
    for (Illusion_Message *&ils_msg : illusion_msg_ary_)
    {
        QString excel_fname = excel_path_.absolutePath() + "/" + ils_msg->excel_file_name_;
        QString outer_fname = outer_path_.absolutePath() + "/" + ils_msg->outer_file_name_;
        QFileInfo excel_finfo(excel_fname);
        if (excel_finfo.exists())
        {
            ils_msg->exist_excel_file_ = true;
            QFileInfo outer_finfo(outer_fname);
            if (!outer_finfo.exists())
            {
                ils_msg->excelcfg_is_newer_ = true;
            }
            else
            {
                qDebug() << excel_finfo.lastModified().toString() << " "
                         << outer_finfo.lastModified().toString();
                if (excel_finfo.lastModified() > outer_finfo.lastModified())
                {
                    ils_msg->excelcfg_is_newer_ = true;
                }
            }
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
    //
    for (Illusion_Message *&ils_msg : illusion_msg_ary_)
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
    QString tip_info;
    auto iter = msgname_2_illusion_map_.find(messge_full_name);
    if (iter == msgname_2_illusion_map_.end())
    {
        tip_info = QString::fromLocal8Bit("[%1] Message �����޷��ҵ������������������.proto�ļ���")
                   .arg(messge_full_name);
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }

    //��Ϊ��Ҫ����message����ת��û�����xls�ļ���ֱ�ӱ���
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

//��ȡ�и��µ�EXCEL�ļ���Message
int Biko_Read_Config::read_newer_message(QStringList &tips_ary)
{
    int ret = 0;
    for (auto iter = illusion_msg_ary_.begin(); iter != illusion_msg_ary_.end(); ++iter)
    {
        if ((*iter)->exist_excel_file_ && (*iter)->excelcfg_is_newer_)
        {
            //��EXCEL�ļ�
            ret = open_excel_file((*iter)->excel_file_name_,
                                  false,
                                  tips_ary);
            if (0 != ret)
            {
                return ret;
            }
            ret = read_excel_table((*iter), tips_ary);
            if (0 != ret)
            {
                close_excel_file();
                return ret;
            }

            //�ر�EXCEL �ļ�
            close_excel_file();
        }
    }
    return 0;
}


//
int Biko_Read_Config::read_proto_file(const QFileInfo &proto_file,
                                      QStringList &tips_ary)
{
    int ret = 0;
    QString tip_info;
    const google::protobuf::FileDescriptor *file_desc = NULL;

    error_collector_.clear_error();
    QString proto_fname;
    proto_fname = proto_file.fileName();

    file_desc = protobuf_importer_->Import(proto_fname.toStdString());
    if (!file_desc)
    {
        tip_info = QString::fromLocal8Bit("Importer Import filename [%1] ʧ��.")
                   .arg(proto_fname);
        illusion::process_tips(tip_info, tips_ary);
        error_collector_.print_tostdout();
        error_collector_.to_qstringlist(tips_ary);
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
            ret = ils_ptr->initialize(proto_file.absoluteFilePath(),
                                      table_msg_desc,
                                      tips_ary);
            if (0 != ret)
            {
                tip_info = QString::fromLocal8Bit("Message [%1] ����չ��Ϣ�д������ȱ�𣬲���ת����һ����ȡ����.").
                           arg(table_msg_desc->full_name().c_str());
                illusion::process_tips(tip_info, tips_ary);
                return ret;
            }

            Illusion_Message *ok_ptr = ils_ptr.get();
            ils_ptr.release();

            //�����Ǽ�¼���ֽ���������ӳ���ϵ
            illusion_msg_ary_.push_back(ok_ptr);

            QString msg_name = ok_ptr->table_message_name_;
            msgname_2_illusion_map_[msg_name] = ok_ptr;

            //�������MAP ���飬����
            auto iter1 = proto_2_illusion_map_.find(proto_fname);
            if (iter1 == proto_2_illusion_map_.end())
            {
                std::vector<Illusion_Message *> ils_msg_ary;
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
                std::vector<Illusion_Message *> ils_msg_ary;
                ils_msg_ary.push_back(ok_ptr);
                excel_2_illusion_map_[excel_fname] = ils_msg_ary;
            }
            else
            {
                iter2->second.push_back(ok_ptr);
            }

        }
    }
    return 0;
}

//��ȡ���е�ö��ֵ
int Biko_Read_Config::read_table_enum(MAP_QSTRING_TO_QSTRING &enum_map,
                                      QStringList &tips_ary)
{
    QString tip_info;
    //���Ƿ���loead ���sheet
    bool bret =  ils_excel_file_.loadSheet("ENUM_CONFIG", true);
    if (bret == false)
    {
        tips_ary.append(QString::fromLocal8Bit("��ѡ�������EXCEL�����ܶ�ȡ�����ñ�[ENUM_CONFIG]."
                                               "û��ö��ֵ��Ҫ��ȡ��"));
        return -1;
    }

    //��Ӧ����
    int row_count = ils_excel_file_.rowCount();
    int col_count = ils_excel_file_.columnCount();
    tip_info = QString::fromLocal8Bit("ENUM_CONFIG ������ [%1] ��[%1]. \n").
               arg(col_count).
               arg(row_count);
    illusion::process_tips(tip_info, tips_ary);
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
    QString tip_info;
    QString excel_path = excel_path_.absolutePath() + "/" + excel_file_name;
    bool bret = ils_excel_file_.open(excel_path, not_exist_new);
    //Excel�ļ���ʧ��
    if (bret != true)
    {
        tip_info = QString::fromLocal8Bit("!ActiveX ��EXCEL �ļ�[%1]ʧ�ܣ�ʹ��OLE��ȡEXCEL���밲װ��EXCEL��").
                   arg(excel_file_name);
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit(".OLE ��һ�����������Ƶ�����������̵ĵ䷶��������ʲô���ⶼ�п��ܡ�");
        illusion::process_tips(tip_info, tips_ary);
        tip_info = QString::fromLocal8Bit(".��װ���EXCEL �������á�����ѣ�����˭ȥ��");
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }
    tip_info = QString::fromLocal8Bit(".ActiveX ��EXCEL �ļ�[%1]�ɹ�������sheet ����[%2]��").
               arg(excel_file_name).
               arg(ils_excel_file_.sheetsCount());
    illusion::process_tips(tip_info, tips_ary);
    return 0;
}

void Biko_Read_Config::close_excel_file()
{
    ils_excel_file_.close();
}

//��ȡһ��EXCEL���е�����
int Biko_Read_Config::read_excel_table(Illusion_Message *ils_msg,
                                       QStringList &tips_ary)
{
    int ret = 0;
    QString tip_info;
    //���EXCEL�ļ����Ƿ���������
    if (ils_excel_file_.loadSheet(ils_msg->excel_sheet_name_, true) == false)
    {
        tip_info = QString::fromLocal8Bit("!��ѡ�������EXCEL�ļ�[%1]�е�"
                                          "[%2]�����ڻ��߲���ȷ�������ּ���򿪡�!")
                   .arg(ils_msg->excel_file_name_)
                   .arg(ils_msg->excel_sheet_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -3;
    }

    int line_count = ils_excel_file_.rowCount();
    int col_count = ils_excel_file_.columnCount();

    tip_info = QString::fromLocal8Bit(".��ȡEXCEL�ļ�:[%1]��(Sheet) :[%2]��ʼ. ���� [%3] ���� [%4]."
                                      "����[%5]��ȡ. ������[%6] ��ȡ������[%7]").
               arg(ils_msg->excel_file_name_).
               arg(ils_msg->excel_sheet_name_).
               arg(line_count).
               arg(col_count).
               arg(ils_msg->read_data_line_).
               arg(ils_msg->fieldsname_line_).
               arg(ils_msg->column_field_count_);
    illusion::process_tips(tip_info, tips_ary);

    //�����ʶ��pb�ֶ��еȣ�����ʵû����ô����
    if (ils_msg->read_data_line_ > line_count ||
        ils_msg->fieldsname_line_ > line_count ||
        ils_msg->column_field_count_ > col_count)
    {
        tip_info = QString::fromLocal8Bit("!��ѡ�������EXCEL[%1]�ļ��е�"
                                          "[%2]��û���㹻�������Թ���ȡ��!").
                   arg(ils_msg->excel_file_name_).
                   arg(ils_msg->excel_sheet_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -4;
    }

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
            if (first_char == '#' || first_char == '%' || first_char == '!')
            {
                continue;
            }
            else
            {
                read_col.push_back(col_no);
            }
        }
        if (read_col.size() != static_cast<size_t>(ils_msg->column_field_count_))
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
                                    table_msg,
                                    tips_ary);
    if (0 != ret || NULL == table_msg)
    {
        return -2;
    }
    std::unique_ptr<google::protobuf::Message> ptr_msg(table_msg);

#if defined _DEBUG || defined DEBUG
    for (size_t s = 0; s < read_col.size(); ++s)
    {

        tip_info = QString::fromLocal8Bit(".��ȡ���а��� �� [%1] full name [%2] ").
                   arg(read_col[s]).
                   arg(ils_msg->line_field_desc_ary_[s]->full_name().c_str());
        illusion::process_tips(tip_info, tips_ary);
    }
#endif

    int read_line = 0;
    for (int line_no = ils_msg->read_data_line_; line_no <= line_count; ++line_no)
    {
        std::vector<QString > line_data_ary;
        line_data_ary.reserve(col_count);
        //fprintf(stderr, "Read line [%d] \n", line_no);
        bool null_line = true;
        for (size_t p = 0; p < read_col.size(); ++p )
        {
            QString cell_data = ils_excel_file_.getCell(line_no,
                                                        read_col[p]).toString();
            if (cell_data.isEmpty() == false)
            {
                null_line = false;
            }
            line_data_ary.push_back(cell_data);
        }
        //��������
        if (true == null_line)
        {
            tip_info = QString::fromLocal8Bit(".EXCEL�ļ�[%1]TABLE (Sheet)[%2]�еĵ�[%3]���ǿ�������(ȫ����ΪNULL)��"
                                              "�����������������")
                       .arg(ils_msg->excel_file_name_)
                       .arg(ils_msg->excel_sheet_name_)
                       .arg(line_no);
            illusion::process_tips(tip_info, tips_ary);
            continue;
        }
        int error_field_no = -1;
        const google::protobuf::FieldDescriptor *error_field_desc = NULL;
        ret = ils_msg->add_line(table_msg,
                                line_data_ary,
                                error_field_no,
                                error_field_desc);
        if (0 != ret)
        {
            tip_info = QString::fromLocal8Bit("!��̬д��Messageʧ�ܣ�Table Message [%1] Line Message [%2] �ֶ�����[%3]."
                                              "��Ӧ�ֶ��ǵ�EXCEL �ļ�[%4]Sheet ����[%5].�к��к�[%6|%7]%8")
                       .arg(ils_msg->table_message_name_)
                       .arg(ils_msg->line_message_name_)
                       .arg(error_field_desc->full_name().c_str())
                       .arg(ils_msg->excel_file_name_)
                       .arg(ils_msg->excel_sheet_name_)
                       .arg(line_no)
                       .arg(read_col[error_field_no])
                       .arg(QtAxExcelEngine::columnName(read_col[error_field_no]));
            illusion::process_tips(tip_info, tips_ary);
            return ret;
        }
        ++read_line;
    }
    //���û�г�ʼ��
    if (!table_msg->IsInitialized())
    {
        tip_info = QString::fromLocal8Bit("!��ȡ��TABLE Message [%1] IsInitialized ����false,������Ϣ��[%2]."
                                          "�������EXCEL��Proto�ļ�.").
                   arg(ils_msg->table_message_name_).
                   arg(table_msg->InitializationErrorString().c_str());
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }

    tip_info = QString::fromLocal8Bit(".��ȡ��EXCEL [%1] ��(Sheet)[%2] ����������ȡ[%3]��.").
               arg(ils_msg->excel_file_name_).
               arg(ils_msg->excel_sheet_name_).
               arg(line_count - ils_msg->read_data_line_ + 1);
    illusion::process_tips(tip_info, tips_ary);

    std::cout << table_msg->DebugString() << std::endl;

    ret = save_to_protocfg(ils_msg, table_msg, tips_ary);
    if (0 != ret)
    {
        return ret;
    }
    ils_msg->excelcfg_is_newer_ = false;
    return 0;
}

//
int Biko_Read_Config::save_excel_tablehead(const QString &messge_full_name,
                                           QStringList &tips_ary)
{
    int ret = 0;

    //ͨ��ȫ·��message���֣��ҵ���Ҫת���ģ��Ѿ��������õ�message
    auto iter = msgname_2_illusion_map_.find(messge_full_name);
    if (iter == msgname_2_illusion_map_.end())
    {
        return -1;
    }
    const Illusion_Message *ils_msg = iter->second;

    //��EXCEL�ļ�,û�оʹ���
    ret = open_excel_file(ils_msg->excel_file_name_,
                          true,
                          tips_ary);
    if (0 != ret)
    {
        return ret;
    }

    //���EXCEL�ļ����Ƿ���������,�����������
    if (ils_excel_file_.loadSheet(ils_msg->excel_sheet_name_, true) == true)
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
    QString tip_info;
    Q_ASSERT(table_msg->IsInitialized());
    //ǰ��Ӧ�ü�����
    if (!table_msg->IsInitialized())
    {
        tip_info = QString::fromLocal8Bit("!�����TABLE Message [%1] IsInitialized ����false,������Ϣ��[%2].").
                   arg(ils_msg->table_message_name_).
                   arg(table_msg->InitializationErrorString().c_str());
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }

    QString pbc_file = outer_path_.path() + "/";
    pbc_file += ils_msg->outer_file_name_;
    QString txt_file = pbc_file + ".txt";
    QFile pbc_config(pbc_file);
    pbc_config.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!pbc_config.isWritable())
    {
        tip_info = QString::fromLocal8Bit("!������ļ� [%1] �޷�д��.Table message[%1]").
                   arg(pbc_file).
                   arg(ils_msg->table_message_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }
    QFile txt_config(txt_file);
    txt_config.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!txt_config.isWritable())
    {
        tip_info = QString::fromLocal8Bit("!������ļ� [%1] �޷�д�룬Table message[%1].").
                   arg(txt_file).
                   arg(ils_msg->table_message_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -1;
    }

    std::string bin_string, txt_string;
    bool succ = table_msg->SerializeToString(&bin_string);
    if (!succ)
    {
        tip_info = QString::fromLocal8Bit("!Table message[%1]SerializeToString  ����ʧ��.").
                   arg(ils_msg->table_message_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -2;
    }
    google::protobuf::TextFormat::Printer printer;
    printer.SetUseUtf8StringEscaping(true);
    succ = printer.PrintToString(*table_msg, &txt_string);
    if (!succ)
    {
        tip_info = QString::fromLocal8Bit("!Table message[%1] PrintToString  ����ʧ��.").
                   arg(ils_msg->table_message_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -2;
    }
    qint64 wt_len = pbc_config.write(bin_string.c_str(), bin_string.length());
    if (wt_len < 0)
    {
        tip_info = QString::fromLocal8Bit("!������ļ� [%1] д��ʧ��.Table message[%1]").
                   arg(pbc_file).
                   arg(ils_msg->table_message_name_);
        illusion::process_tips(tip_info, tips_ary);
        return -3;
    }
    pbc_config.close();

    wt_len = txt_config.write(txt_string.c_str(), txt_string.length());
    if (wt_len < 0)
    {
        tip_info = QString::fromLocal8Bit("!������ļ� [%1] д��ʧ��.Table message[%1]").
                   arg(txt_file).
                   arg(ils_msg->table_message_name_);
        return -4;
    }
    txt_config.close();
    return 0;
}



QSTRING_2_ILLUSIONARY_MAP *Biko_Read_Config::get_proto_illusion_map()
{
    return &proto_2_illusion_map_;
}


ILLUSION_MESSAGE_ARRAY *Biko_Read_Config::get_illusion_sheetmsg_ary()
{
    return &illusion_msg_ary_;
}



//!ȡ��Outer�ļ���Ŀ¼·��
QString Biko_Read_Config::outer_path()
{
    return outer_path_.absolutePath();
}

//!ȡ��Excel�ļ���Ŀ¼·��
QString Biko_Read_Config::excel_path()
{
    return excel_path_.absolutePath();
}

//!ȡ��Proto�ļ���Ŀ¼·��
QString Biko_Read_Config::proto_path()
{
    return proto_path_.absolutePath();
}




