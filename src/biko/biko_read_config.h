#pragma once


#include "biko_qt_excel_engine.h"
#include "biko_illusion_message.h"



//===========================================================================================
//
//
struct ZCE_PROTO_ERROR
{
    std::string file_name_;
    int line_;
    int column_;
    std::string message_;
};

typedef std::vector<ZCE_PROTO_ERROR> PROTO_ERROR_ARRAY;

//�����ռ�
class ZCE_Error_Collector : public google::protobuf::compiler::MultiFileErrorCollector
{
public:
    ZCE_Error_Collector();
    virtual ~ZCE_Error_Collector();

public:

    virtual void AddError(const std::string &filename,
                          int line,
                          int column,
                          const std::string &message);

    void clear_error();

public:
    //
    PROTO_ERROR_ARRAY error_array_;
};

//===========================================================================================
class Illusion_Message;


class Biko_Read_Config
{
protected:


    //!ö��ֵ�Ķ�Ӧ��ϵ��
    typedef std::map <QString, QString >  MAP_QSTRING_TO_QSTRING;

protected: // �������л�����
    Biko_Read_Config();
protected:
    virtual ~Biko_Read_Config();


public:

    //!
    static Biko_Read_Config *instance();

    //!
    static void clean_instance();

public:

    /*!
    * @brief      ��ʼ����ȡ������׼������Ŀ¼������ת��
    * @return     int
	* @param[in]  proto_dir ����proto_dirĿ¼�µ�meta�ļ����䣬
    * @param[in]  excel_dir ��ȡexcel_dirĿ¼�����е�EXCEL�ļ�
    * @param[in]  outer_dir ת����λ���ļ������outer_dirĿ¼�����ΪNULL�����ʾ�õ�ǰĿ¼���
    */
    int init_read_all(const QString &proto_dir,
                      const QString &excel_dir, 
                      const QString &outer_dir,
                      QStringList &error_tips);

    //!���е�Ŀ¼����һ��Ŀ¼�µĿ�ݴ���ʽ
    int init_read_all2(const QString &allinone_dir,
                       QStringList &error_tips);

    //!12312
    void finalize();


	int read_all_message(QStringList &tips_info);


	int read_one_message(const QString &messge_full_name,
						 QStringList &error_tips);

    //�������еĶ�ȡ����
    void clear();


protected:

    //��ö��ֵ
    int read_table_enum(MAP_QSTRING_TO_QSTRING &enum_map);

    /*!
    * @brief
    * @return     int
    * @param      outer_dir
    * @param[out] error_tips ������Ϣ��
    */
    int init_outdir(const QString &outer_dir,
                    QStringList &error_tips);


    /*!
    * @brief      map proto �ļ���Ŀ¼��ͬʱ�����������е�.proto�ļ�
    * @return     int
    * @param      proto_dir
    * @param[out] error_tips ������Ϣ��
    */
    int init_protodir(const QString &proto_dir,
                      QStringList &error_tips);


	int init_exceldir(const QString &excel_dir,
					  QStringList &error_tips);




    //!
    int read_proto_file(const QFileInfo &proto_file,
                        QStringList &error_tips);

	//!
	int open_illusion_excel(const QString &excel_file_name,
							bool read_enum_sheet,
						    QStringList &error_tips);

	//!
	int read_illusion_excel(const Illusion_Message *ils_msg,
							QStringList &error_tips);

protected:

    //����ʵ��
    static Biko_Read_Config  *instance_;

    //
    static char REPEATED_STRING[];
protected:

	//!proto�ļ���ŵ�·��
	QDir proto_path_;

    //!EXCEL���ô�ŵ�Ŀ¼
    QDir excel_path_;

    //!��־�����Ŀ¼
    QDir out_log_path_;

    //!PBC�ļ������Ŀ¼·����PBC protobuf config
    QDir out_pbc_path_;


    //!Excel�Ĵ������,EXCEL�Ĵ�����
    QtAxExcelEngine ils_excel_file_;


    //!
    google::protobuf::compiler::Importer *protobuf_importer_ = NULL;
    //!
    google::protobuf::compiler::DiskSourceTree *source_tree_ = NULL;
    //
    google::protobuf::DynamicMessageFactory *msg_factory_ = NULL;
    //
    ZCE_Error_Collector error_collector_;

    //!ÿ��������Message�����õ�Illusion_Message���������������
    std::vector <const Illusion_Message *> illusion_msg_ary_;
	//!outer����ļ����õ�proto���õ���Ϣ
	std::map <QString, const Illusion_Message *> msgname_2_illusion_map_;
    
    //!proto ���ƶ�Ӧ
    std::map <QString, ILLUSION_MESSAGE_ARRAY> proto_cfg_map_;
	//!EXCEL �ļ����ƶ�Ӧproto���õ���Ϣ
	std::map <QString, ILLUSION_MESSAGE_ARRAY> excel_cfg_map_;

    //!outer����ļ����õ�proto���õ���Ϣ
    std::map <QString, const Illusion_Message *> outer_cfg_map_;

    //!EXCEL�ļ��б�
    QFileInfoList excel_fileary_;

    //!proto meta�ļ��б�
    QFileInfoList proto_fileary_;


};


