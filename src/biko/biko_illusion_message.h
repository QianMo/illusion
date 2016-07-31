#pragma once

//===========================================================================================
//
class Illusion_Message
{

public:

    Illusion_Message();
    ~Illusion_Message();


    int init(const google::protobuf::Descriptor *table_msg_desc);


    //!New һ�� table message �����Ǹ���repeated line message ��message
    int new_table_mesage(google::protobuf::DynamicMessageFactory *msg_factory,
                         google::protobuf::Message *&table_msg) const;

    //!
    int add_line(google::protobuf::Message *table_msg,
                 std::vector<QString> &line_str_ary,
				 int &error_field_no,
				 const google::protobuf::FieldDescriptor *&error_field_desc) const;



protected:

    //!��Ϊһ��message��������message������ݹ鴦��
    int recursive_proto(const google::protobuf::Descriptor *msg_desc);
    //!
    int recursive_msgfield(google::protobuf::Message *msg,
                           std::vector<google::protobuf::Message *> *tb_message_ary) const;

public:

    //!
    const google::protobuf::Descriptor *table_msg_desc_;
    //!
    const google::protobuf::Descriptor *line_msg_desc_;

    //!����Ӧ��protobuf��message����
    QString line_message_name_;

    //!��Ӧ��repeat line message �ṹ�����ƣ�
    QString table_message_name_;

    //!�������
    QString excel_file_name_;
    //!�������
    QString excel_sheet_name_;

	//���protobuf�������ݵĵ��ļ�����
	QString outer_file_name_;

	//!EXCEL�ļ������Ǹ��µ�
	bool excelcfg_is_newer_ = false;

    //!
    int fieldsname_line_ = 1;
    //!���ĵڼ��������ֶζ�Ӧ��protobuf
    int fullname_line_ = 2;
    //!������ݴӵڼ��ж�ȡ
    int read_data_line_ = 3;
    

    //!��Ӧ����չ�����ֶ�����������repeated,sub message���ֶ�,���ݹ�չ��������
    int column_field_count_ = 0;

    //!��Ӧ����չ�����ֶε����Ƶ����飬���ڲ���EXCEL�����Ϊ����
    std::vector<QString>  column_fieldname_ary_;
    //!��Ӧ����չ�����ֶε�fullname���飬���ڲ���EXCEL�����Ϊ����2
    std::vector<QString>  column_fullname_ary_;

    //!�ֶ�����ע���ֶ�������Message �Ƕ�Ӧ��ϵ���������ô�����ѣ������ط����ù���
    std::vector<const google::protobuf::FieldDescriptor *> line_field_desc_ary_;


};


//!
typedef std::vector<const Illusion_Message *> ILLUSION_MESSAGE_ARRAY;


typedef std::map <QString, ILLUSION_MESSAGE_ARRAY> QSTRING_2_ILLUSIONARY_MAP;


