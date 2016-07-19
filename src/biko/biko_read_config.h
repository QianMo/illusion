#pragma once


#include "QtAxExcelEngine.h"
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
    int init_read_all(const QStringList &import_list,
		              const QString &proto_dir,
                      const QString &excel_dir,
                      const QString &outer_dir,
                      QStringList &tips_ary);

    //!���е�Ŀ¼����һ��Ŀ¼�µĿ�ݴ���ʽ
    int init_read_all2(const QString &allinone_dir,
                       QStringList &tips_ary);

    //!12312
    void finalize();


    int read_all_message(QStringList &tips_info);


    int read_one_message(const QString &messge_full_name,
                         QStringList &tips_ary);

	//!����EXCEL�ı�ͷ
	int save_excel_tablehead(const QString &messge_full_name,
							 QStringList &tips_ary);

    //�������еĶ�ȡ����
    void clear();


protected:


	
	/*!
	* @brief      Proto ��import��Ŀ¼�������Ƕ��
	* @param      import_dir  import��Ŀ¼�б�
	* @param      tips_ary
	*/
	void init_importdir(const QStringList &import_list,
						QStringList &tips_ary);

    /*!
    * @brief
    * @return     int
    * @param      outer_dir
    * @param[out] tips_ary ������Ϣ��
    */
    int init_outdir(const QString &outer_dir,
                    QStringList &tips_ary);


    /*!
    * @brief      map proto �ļ���Ŀ¼��ͬʱ�����������е�.proto�ļ�
    * @return     int
    * @param      proto_dir
    * @param[out] tips_ary ������Ϣ��
    */
    int init_protodir(const QString &proto_dir,
                      QStringList &tips_ary);


    /*!
    * @brief      
    * @return     int
    * @param      excel_dir EXCEL Ŀ¼
    * @param      tips_ary  ��ʾ��Ϣ
    */
    int init_exceldir(const QString &excel_dir,
                      QStringList &tips_ary);


    //!
    int read_proto_file(const QFileInfo &proto_file,
                        QStringList &tips_ary);

    //!��EXCEL�ļ�
    int open_excel_file(const QString &excel_file_name,
                        bool not_exist_new,
                        QStringList &tips_ary);
    
	//�ر�EXCEL �ļ�
	void close_excel_file();

	//��ö��ֵ
	int read_table_enum(MAP_QSTRING_TO_QSTRING &enum_map,
						QStringList &tips_info);

    //!
    int read_excel_table(const Illusion_Message *ils_msg,
                         QStringList &tips_ary);



    /*!
    * @brief      �����ݱ��浽Proto buf config �����ļ�����
    * @return     int
    * @param      ils_msg
    * @param      table_cfg
    * @param      tips_ary
    */
    int save_to_protocfg(const Illusion_Message *ils_msg,
                         const google::protobuf::Message *table_msg,
                         QStringList &tips_ary) const;

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

    //!ÿ��������Message�����õ�Illusion_Message���������������,
    std::vector <const Illusion_Message *> illusion_msg_ary_;

    //!ÿ��message�� full name ��Ӧ��Illusion_Message ������ͨ��������Illusion_Message
    std::map <QString, const Illusion_Message *> msgname_2_illusion_map_;

    //!proto �ļ����ƶ�Ӧ Illusion_Message �б�һ��.proto�����ж��Illusion_Message
    std::map <QString, ILLUSION_MESSAGE_ARRAY> protoname_2_illusion_map_;

    //!EXCEL �ļ����ƶ�Ӧproto���õ���Ϣ
    std::map <QString, ILLUSION_MESSAGE_ARRAY> excelname_2_illusion_map_;

    //!outer����ļ����õ�proto���õ���Ϣ�����ڲ�ѯ
    std::map <QString, const Illusion_Message *> outername_2_illusion_map_;

    //!EXCEL�ļ��б�
    QFileInfoList excel_fileary_;

    //!proto meta�ļ��б�
    QFileInfoList proto_fileary_;


};


