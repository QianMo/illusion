/*!
* @copyright  2004-2014  Apache License, Version 2.0 FULLSAIL
* @filename   zce_protobuf_reflect.h
* @author     Sailzeng <sailerzeng@gmail.com>
* @version
* @date       Tuesday, November 25, 2014
* @brief
*
*
* @details
*
*
*
* @note
* ������֣�ݵļ��䡷 ��־
* ����֣����֪���Ĳ��࣬Ϊ�˰�������ȥ������
* ���ٴ��ڻ���·������У�һ�������ĵ�������
* ��˵��ϲ��֣�ݶ�������⣬������Ʈ��ú¯��ζ��
* ��������������Ĳ��ӣ�ֱ�����춼û��ɢȥ
* ����֣�������ȫ���㣬������ȥ������ں�ί��
* ����֣���Ұ���ȫ���㣬������ȥ�����װ�������
* ����֣��ֻ��ż����������ζ�����ڻ�����
* ÿ�κ�����˵���ȥ�����У��Ҳ���˵����ȥ������
* ����֣�������ȫ���㣬���������޷���ʹ�������
* ����֣���Ұ���ȫ���㣬����������Ƕ���·��ȥ
* ���Ƕ��ǻ������¿�η�������������������޴�
* ʱ��ı��˺ܶ���ʲô��û��
* �����ٴ�ӵ���㣬֣��
*/

#ifndef _ILLUSION_PROTOBUF_REFLECT_H_
#define _ILLUSION_PROTOBUF_REFLECT_H_



//======================================================================================

/*!
* @brief
*
* @note
*/
class Protobuf_Reflect_AUX
{
    //
public:


    /*!
    * @brief      ����fullname��Ҳ���� phone_book.number ����һ��Message��field
    * @return     int
    * @param      msg          ��ӦҪ������msg
    * @param      full_name    �ֶε�ȫ��������Ƕ���ṹ�ֶο�����.����
    * @param      set_data     ���õ�����
    * @param      repeated_add �Ƿ���repeated �ֶΣ��ǽ�����Ӳ���
    */
    static int set_field(google::protobuf::Message *msg,
                         const std::string &full_name,
                         const std::string &set_data,
                         bool repeated_add);

    //!����fullname,�õ�ĳ���ֶε�������Ϣ
    static int get_fielddesc(google::protobuf::Message *msg,
                             const std::string &full_name,
                             bool message_add,
                             google::protobuf::Message *&field_msg,
                             const google::protobuf::FieldDescriptor *&field_desc);

    //!����һ��Message��field
    static int set_fielddata(google::protobuf::Message *msg,
                             const google::protobuf::FieldDescriptor *field,
                             const std::string &set_data);

	static int set_fielddata(google::protobuf::Message *msg,
							 const google::protobuf::FieldDescriptor *field,
							 const QString &set_data);

    //!��λһ���ӽṹ
    static int locate_sub_msg(google::protobuf::Message *msg,
                              const std::string &submsg_field_name,
                              bool message_add,
                              google::protobuf::Message *&sub_msg);


    //!��λһ���ӽṹ
    static int locate_msgfield(google::protobuf::Message *msg,
                               const google::protobuf::FieldDescriptor *msg_field,
                               google::protobuf::Message *&sub_msg,
                               bool message_add);

    //!��ӡ���һ��Message����Ϣ��ostream���棬
    static void protobuf_output(const google::protobuf::Message *msg,
                                std::ostream *out);

    //!��message�������е��ֶ�����ΪĬ��ֵ��
    //!ע�������ֱ����Clear������ԭ���ǣ����ڶ�ȡ���õĹ����Ѿ���Message�ṹ�������
    //!��Ӧ��Field��ָ��Ҳ�����ˣ����Clear��һ�ж�ʧЧ�ˣ���repeated����������Mesage����
    static void message_set_default(google::protobuf::Message *msg);


    //!
    static bool string_to_bool(const std::string &str);

	//!
	static bool qstring_to_bool(const QString &str);

    static void string_split(const std::string &source_str,
                             const std::string &separator,
                             std::vector<std::string> &v);




};



#endif //# _ILLUSION_PROTOBUF_REFLECT_H_


