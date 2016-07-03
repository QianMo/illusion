#pragma once

//===========================================================================================
//
class Illusion_Message
{

public:

	Illusion_Message();
	~Illusion_Message();


	int init(const google::protobuf::Descriptor *table_msg_desc);


	//!
	int new_table_mesage(google::protobuf::DynamicMessageFactory *msg_factory,
		                 google::protobuf::Message *&table_msg);

	//!
	int add_line(google::protobuf::Message *table_msg,
				 std::vector<std::string> &line_str_ary);

protected:

	//!��Ϊһ��message��������message������ݹ鴦��
	int recursive_proto(const google::protobuf::Descriptor *msg_desc,
						int &field_count);

	
protected:

	//!
	const google::protobuf::Descriptor *table_msg_desc_;
	//!
	const google::protobuf::Descriptor *line_msg_desc_;

	//!����Ӧ��protobuf��message����
	QString line_messge_name_;

	//!��Ӧ��repeat line message �ṹ�����ƣ�
	QString list_messge_name_;

	//!�������
	QString excel_filename_;
	//!�������
	QString excel_sheetname_;

	//!
	int fieldsname_line_ = 1;
	//!���ĵڼ��������ֶζ�Ӧ��protobuf
	int fullname_line_ = 2;
	//!������ݴӵڼ��ж�ȡ
	int read_data_line_ = 3;

	

	//���protobuf�������ݵĵ��ļ�����
	QString outer_file_name_;

	//!��Ӧ����չ�����ֶ�����������repeated,sub message���ֶ�,���ݹ�չ��������
	int tb_field_count_ = 0;

	//!��Ӧ����չ�����ֶε����Ƶ�����
	std::vector<QString>  tb_fieldname_ary_;
	//!��Ӧ����չ�����ֶε�fullname����
	std::vector<QString>  tb_fullname_ary_;
	//!
	std::vector<const google::protobuf::FieldDescriptor *> tb_field_desc_ary_;

};


