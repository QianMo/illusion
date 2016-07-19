
/*!
* @copyright  2004-2016  Apache License, Version 2.0 FULLSAIL
* @filename   QtAxExcelEngine.h
* @author     yaoboyuan 254200341@qq.com ԭ��    Sailzeng <sailerzeng@gmail.com> ��д
* @version
* @date       2016��6��10��
* @brief      ����һ��ʹ�� Qt's ActiveX(OLE)��QAxObject ��дEXCEL�ķ�װ�࣬���ڲ�����ʹ��Qt
*             �Ĵ������EXCEL����Ȼ��ʵ��OLE,�㵱Ȼ�����Ȱ�װEXCEL��
*             
*             ����һ������Qt��дexcel��װ���࣬ͬʱ�����ڰ�excel�е�����
*             ��ʾ�������ϣ����߰ѽ����ϵ�����д��excel�У�ͬGUI���н�������ϵ���£�
*             Qt tableWidget <--> ExcelEngine <--> xls file.
* @details
*
*
*
* @note       ExcelEngine��ֻ�����/д���ݣ���������������м��
*
* @history
* @author     yaoboyuan 254200341@qq.com
* @date       2012-4-12
* @note       ԭ��
* @author     Sailzeng ��д <sailerzeng@gmail.com>
* @data       2016-6-7 �����ǰϦ
* @note       ��ԭ�еĴ�������һЩ�Ķ������ӷ���Qt�ı����ʽ�ȡ�
*             ������һЩ���ú��������ӷ���ʵ��
*
*/


#ifndef QT_ACTIVEX_EXCELENGINE_H
#define QT_ACTIVEX_EXCELENGINE_H



/*!
* @brief
*
* note       �������Ҵ���ȫ������Qt�Ĵ�����
*/
class QtAxExcelEngine : protected QObject
{
public:

    //���캯������������
    QtAxExcelEngine();
    ~QtAxExcelEngine();

public:


    /*!
    * @brief      ��ʼ��EXCEL OLE���󣬴�EXCEL ���̣�
    * @return     bool  �����Ƿ�ɹ���
    * @param      visible �Ƿ���EXCEL�ɼ�
    */
    bool initialize(bool visible);

    //!�ͷ��˳����ر�EXCEL����
    void finalize();

	/*!
	* @brief      �򿪣��½���һ��XLS�ļ�
	* @return     bool          true��ʾ�򿪳ɹ�
	* @param      xls_file      �򿪵��ļ�����
	* @param      not_exist_new �������Ƿ񴴽�һ���µ�
	*/
	bool open(const QString &xls_file,
			  bool not_exist_new);



    //����xls����
    void save();

    //�ر�xls����
    void close();

    //�õ�sheet������
    int sheetsCount();

    //�õ�ĳ��sheet������
    bool sheetName(int sheet_index, QString &sheet_name);

    /*!
    * @brief      ������������sheet��
    * @return     bool �����Ƿ�ɹ�����
    * @param      sheet_index sheet��������1��ʼ
	* @param      pre_load �Ƿ����Ԥ���أ�Ԥ���ػ��ȡ����EXCEL��sheet���ӿ����Ķ�ȡ����
    */
    bool loadSheet(int sheet_index,
				   bool pre_load = false);

    /*!
    * @brief      ����sheet����������sheet��
    * @return     bool �����Ƿ�ɹ�����
    * @param      sheet_name Ҫ���ص�sheet ������
	* @param      pre_load �Ƿ����Ԥ���أ�Ԥ���ػ��ȡ����EXCEL��sheet���ӿ����Ķ�ȡ����
    */
    bool loadSheet(const QString &sheet_name,
				   bool pre_load = false);


    /*!
    * @brief      ����Ƿ���������ֵ�sheet
    * @return     bool �з���true�����򷵻�false
    * @param      sheet_name
    */
    bool hasSheet(const QString &sheet_name);


	/*!
	* @brief      ����һ��sheet��ͬʱ����������
	* @param      sheet_name sheet�ı���
	*/
	void insertSheet(const QString &sheet_name);

	
	/*!
	* @brief      �Ե�ǰ��Sheet���и���������
	* @param      new_name �µ�����
	*/
	void renameSheet(const QString &new_name);


    /*!
    * @brief      ��tableWidget�е����ݱ��浽excel��
    * @return     bool
    * @param      table_widget
    */
    bool writeTableWidget(QTableWidget *table_widget);

    //��xls��ȡ���ݵ�ui
    bool readTableWidget(QTableWidget *table_widget);

    /*!
    * @brief      ��ȡָ����Ԫ����
    * @return     QVariant  ��Ԫ���Ӧ������
    * @param      row  ��Ԫ����к�,ע���кţ��кŶ��Ǵ�1��ʼ
    * @param      column ��Ԫ����к�
    * @note
    */
    QVariant getCell(int row, int column);

    
    /*!
    * @brief      �޸�ָ����Ԫ����
    * @return     bool  �Ƿ��޸ĳɹ�
    * @param      row    �к�
    * @param      column �к�
    * @param      data   �޸ĵ�����
    */
    bool  setCell(int row, int column, const QVariant &data);

    //!�򿪵�xls�ļ�����
    QString openFilename() const;

    //!��ǰSheet������,�������� 
    int  rowCount() const;
    //!��ǰSheet������,�������� 
    int  columnCount() const;

	//!��ǰ��Sheet����ʼ�����������1,2���ǿգ�û�����ݣ���ô����3
	int startRow() const;
	//!��ǰ��Sheet����ʼ�����������1,2,3���ǿգ�û�����ݣ���ô����4
	int startColumn() const;


    bool is_open();
    bool is_valid();

protected:

    //!���أ��ڲ��������Ժ���Կ�������һ��Ԥ���أ��ӿ��ȡ�ٶȡ�
    void loadSheet_internal(bool pre_load);

public:

    /*!
    * @brief      ȡ���е����ƣ�����27->AA
    * @return     QString
    * @param      column_no
    */
    static QString columnName(int column_no);

private:

    //!ָ������excelӦ�ó���
    QAxObject *excel_instance_ = NULL;

    //!ָ��������,excel�кܶ๤����������Լ����EXCEL��򿪺ܶ���ļ�
    QAxObject *work_books_ = NULL;

    //!ָ��sXlsFile��Ӧ�Ĺ�����
    QAxObject *active_book_ = NULL;

    //ָ�������е�ĳ��sheet��
    QAxObject *active_sheet_ = NULL;

    //!xls�ļ�·��
    QString xls_file_;

	//Ԥ���صı������
	QVariantList preload_sheet_data_;

    //!excel�Ƿ�ɼ�
    bool is_visible_ = false;

    //!����,������ʼ�Ŀ���
    int row_count_ = 0;
    //!����,������ʼ�Ŀ���
    int column_count_ = 0;

    //!��ʼ�����ݵ����±�ֵ
    int start_row_ = 1;
    //!��ʼ�����ݵ����±�ֵ
    int start_column_ = 1;

    //!�Ƿ��Ѵ�
    bool is_open_ = false;
    //!�Ƿ���Ч
    bool is_valid_ = false;
    //!�Ƿ���һ���½�xls�ļ����������ִ򿪵�excel���Ѵ����ļ������б����½���
    bool is_a_newfile_ = false;
    //!��ֹ�ظ�����
    bool is_save_already_ = false;

	

};

#endif // QT_EXCELENGINE_H
