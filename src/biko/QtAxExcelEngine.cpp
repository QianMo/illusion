
#include "QtAxExcelEngine.h"


QtAxExcelEngine::QtAxExcelEngine()
{
    excel_instance_     = NULL;
    work_books_ = NULL;
    active_book_  = NULL;
    active_sheet_ = NULL;
}

QtAxExcelEngine::~QtAxExcelEngine()
{
    if ( is_open_ )
    {
        //����ǰ���ȱ������ݣ�Ȼ��ر�workbook
        close();
    }
    finalize();
}


//��ʼ��EXCEL OLE���󣬴�EXCEL ���̣�
bool QtAxExcelEngine::initialize(bool visible)
{

    HRESULT r = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (r != S_OK && r != S_FALSE)
    {
        qDebug("Qt: Could not initialize OLE (error %x)", (unsigned int)r);
        return false;
    }
    is_visible_ = visible;
    //
    if (NULL == excel_instance_)
    {
        excel_instance_ = new QAxObject("Excel.Application");
        if (excel_instance_->isNull())
        {
            is_valid_ = false;
            is_open_ = false;
            return is_open_;
        }
        else
        {
            is_valid_ = true;
        }

        excel_instance_->dynamicCall("SetVisible(bool)", is_visible_);
    }
    return true;
}


//ע��
void QtAxExcelEngine::finalize()
{
    if (excel_instance_ )
    {

        excel_instance_->dynamicCall("Quit()");

        delete excel_instance_;
        excel_instance_ = NULL;

        is_open_ = false;
        is_valid_ = false;
        is_a_newfile_ = false;
        is_save_already_ = true;

		row_count_ = 0;
		column_count_ = 0;
		start_row_ = 1;
		start_column_ = 1;

		xls_file_ = "";
    }

    ::CoUninitialize();
}


//��EXCEL�ļ�
bool QtAxExcelEngine::open(const QString &xls_file)
{
    xls_file_ = xls_file;
    
    if (is_open_)
    {
        close();
    }

    if (!is_valid_)
    {
        is_open_ = false;
        return is_open_;
    }

    //���ָ����ļ������ڣ�����Ҫ�½�һ��
    QFileInfo fi(xls_file_);
    if (!fi.exists())
    {
        is_a_newfile_ = true;
    }
    else
    {
        is_a_newfile_ = false;
    }

    work_books_ = excel_instance_->querySubObject("WorkBooks");
    if (!is_a_newfile_)
    {
        //��xls��Ӧ�ģ���ȡ������,ע�⣬����õĲ���xls_file_��Ҫ�þ���·��
        active_book_ = work_books_->querySubObject("Open(const QString&,QVariant)", 
												   fi.absoluteFilePath(),
												   0);
    }
    else
    {
        //�½�һ��xls�����һ���µĹ�����
        work_books_->dynamicCall("Add()");
        active_book_ = excel_instance_->querySubObject("ActiveWorkBook");
    }

	if (!active_book_)
	{
		return false;
	}

    is_open_ = true;
    return is_open_;
}

/**
  *@brief ���������ݣ�������д���ļ�
  */
void QtAxExcelEngine::save()
{
    if ( active_book_ )
    {
        if (is_save_already_)
        {
            return ;
        }

        if (!is_a_newfile_)
        {
            active_book_->dynamicCall("Save()");
        }
        else     /*������ĵ����½������ģ���ʹ�����ΪCOM�ӿ�*/
        {
            active_book_->dynamicCall("SaveAs (const QString&,int,const QString&,const QString&,bool,bool)",
                                      xls_file_, 56, QString(""), QString(""), false, false);

        }

        is_save_already_ = true;
    }
}

/**
  *@brief �ر�ǰ�ȱ������ݣ�Ȼ��رյ�ǰExcel COM���󣬲��ͷ��ڴ�
  */
void QtAxExcelEngine::close()
{
    //�ر�ǰ�ȱ�������
    save();
    if ( excel_instance_ && active_book_ )
    {
        active_book_->dynamicCall("Close(bool)", true);

        is_open_     = false;
        is_valid_    = false;
        is_a_newfile_ = false;
        is_save_already_ = true;
    }
}

//
int QtAxExcelEngine::sheetsCount()
{
    return work_books_->property("Count").toInt();
}

//�õ�ĳ��sheet������
bool QtAxExcelEngine::sheetName(int sheet_index, QString &sheet_name)
{
    QAxObject *sheet_tmp = active_book_->querySubObject("WorkSheets(int)", sheet_index);
    if (!sheet_tmp)
    {
        return false;
    }
    sheet_name = sheet_tmp->property("Name").toString();
    return true;
}

bool QtAxExcelEngine::loadSheet(int sheet_index,
								bool pre_load)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(int)", sheet_index);

    //���û�д򿪣�
    if (!active_sheet_)
    {
        return false;
    }
    loadSheet_internal(pre_load);
    return true;
}



//������ż���Sheet���,
bool QtAxExcelEngine::loadSheet(const QString &sheet_name,
								bool pre_load)
{
    active_sheet_ = active_book_->querySubObject("WorkSheets(QString)", sheet_name);
    //���û�д򿪣�
    if (!active_sheet_)
    {
        return false;
    }
    loadSheet_internal(pre_load);
    return true;
}

//
bool QtAxExcelEngine::hasSheet(const QString &sheet_name)
{
    QAxObject *temp_sheet = active_book_->querySubObject("WorkSheets(QString)", 
														 sheet_name);
    if (!temp_sheet)
    {
        return false;
    }
    return true;
}

void QtAxExcelEngine::loadSheet_internal(bool pre_load)
{
    //��ȡ��sheet��ʹ�÷�Χ����
    QAxObject *used_range = active_sheet_->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");

    //��Ϊexcel���Դ��������������ݶ���һ���Ǵ�1,1��ʼ�����Ҫ��ȡ�������±�
    //��һ�е���ʼλ��
    start_row_ = used_range->property("Row").toInt();
    //��һ�е���ʼλ��
    start_column_ = used_range->property("Column").toInt();
    
	//��ȡ������������⣬Ҳ�����˿��У�������ֵط�������⻹��һ�¡�
    row_count_ = rows->property("Count").toInt() + start_row_ -1;
    //��ȡ����
    column_count_ = columns->property("Count").toInt() + start_column_ -1;

	preload_sheet_data_.clear();
	preload_sheet_data_.reserve(row_count_ * column_count_);
	//Ԥ���ص����ݣ����
	if (pre_load)
	{
		QVariantList row_list = used_range->property("Value2").toList();
		//��һ��ת���õ��������ݣ���Ҫ��ȡһ��.ʵ�ʲ��ԣ����ֻ��1,1һ�����ݣ�Ҳ�������Ԥ���ش���
		for (int i = 0; i < row_list.size(); ++i)
		{
			preload_sheet_data_ += row_list.at(i).toList();
		}
	}
	
	delete rows;
	delete columns;
	delete used_range;
    return;
}


//�򿪵�xls�ļ�����
QString QtAxExcelEngine::openFilename() const
{
    return xls_file_;
}

//����
void QtAxExcelEngine::renameSheet(const QString & new_name)
{
	active_sheet_->setProperty("Name", new_name);
}


//��tableWidget�е����ݱ��浽excel��
bool QtAxExcelEngine::writeTableWidget(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }
    if ( !is_open_ )
    {
        return false;
    }

    int tableR = tableWidget->rowCount();
    int tableC = tableWidget->columnCount();

    //��ȡ��ͷд����һ��
    for (int i = 0; i < tableC; i++)
    {
        if ( tableWidget->horizontalHeaderItem(i) != NULL )
        {
            this->setCell(1, i + 1, tableWidget->horizontalHeaderItem(i)->text());
        }
    }

    //д����
    for (int i = 0; i < tableR; i++)
    {
        for (int j = 0; j < tableC; j++)
        {
            if ( tableWidget->item(i, j) != NULL )
            {
                this->setCell(i + 2, j + 1, tableWidget->item(i, j)->text());
            }
        }
    }

    //����
    save();

    return true;
}

/**
  *@brief ��ָ����xls�ļ��а����ݵ��뵽tableWidget��
  *@param tableWidget : ִ��Ҫ���뵽��tablewidgetָ��
  *@return ����ɹ���� true : �ɹ�
  *                   false: ʧ��
  */
bool QtAxExcelEngine::readTableWidget(QTableWidget *tableWidget)
{
    if ( NULL == tableWidget )
    {
        return false;
    }

    //�Ȱ�table���������
    int tableColumn = tableWidget->columnCount();
    tableWidget->clear();
    for (int n = 0; n < tableColumn; n++)
    {
        tableWidget->removeColumn(0);
    }

    //��ȡexcel�еĵ�һ��������Ϊ��ͷ
    QStringList headerList;
    for (int n = start_column_; n <= column_count_; n++ )
    {
        QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", start_row_, n);
        if ( cell )
        {
            headerList << cell->dynamicCall("Value2()").toString();
        }
    }

    //���´�����ͷ
    tableWidget->setColumnCount(column_count_ - start_column_ +1);
    tableWidget->setHorizontalHeaderLabels(headerList);


    //����������
	//��
    for (int i = start_row_ + 1, r = 0; i <= row_count_; i++, r++ )   
    {
		//��������
        tableWidget->insertRow(r); 
		//��
        for (int j = start_column_, c = 0; j <= column_count_; j++, c++ )   
        {
            QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", i, j ); //��ȡ��Ԫ��

            //��r�����������������
            if ( cell )
            {
                tableWidget->setItem(r, c, new QTableWidgetItem(cell->property("Value2").toString()));
            }
        }
    }

    return true;
}

//�õ�ĳ��cell������
QVariant QtAxExcelEngine::getCell(int row, int column)
{
	//���Ԥ����������,
	if (preload_sheet_data_.size() > 0)
	{
		//������Χ���ؿ�����
		if (row > row_count_ || column > column_count_ || row < start_row_ 	|| column < start_column_)
		{
			return QVariant();
		}
		else
		{
			return preload_sheet_data_.at((row - start_row_)*(column_count_ - start_column_+1)
										  + column - start_column_);
		}
	}
	//���û��Ԥ��������
	else
	{
		//��ȡ��Ԫ�����
		QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)", row, column);
		if (cell)
		{
			return cell->property("Value2");
		}
		else
		{
			return QVariant();
		}
	}
}


//�޸�ָ����Ԫ�������
bool QtAxExcelEngine::setCell(int row, int column, const QVariant &data)
{
	bool op = false;
	//��ȡ��Ԫ�����
	QAxObject *cell = active_sheet_->querySubObject("Cells(int,int)",
													row,
													column);
	//excel ��Ȼֻ�ܲ����ַ��������ͣ��������޷�����
	if (cell)
	{
		QString strData = data.toString();
		cell->dynamicCall("SetValue(const QVariant&)", strData); //�޸ĵ�Ԫ�������
		op = true;
	}
	else
	{
		op = false;
	}

	return op;
}



//�ж�excel�Ƿ��ѱ���
bool QtAxExcelEngine::is_open()
{
    return is_open_;
}

/**
  *@brief �ж�excel COM�����Ƿ���óɹ���excel�Ƿ����
  *@return true : ����
  *        false: ������
  */
bool QtAxExcelEngine::is_valid()
{
    return is_valid_;
}


//��ȡexcel������,�������� 
int QtAxExcelEngine::rowCount()const
{
    return row_count_ ;
}


//��ȡexcel������,��������
int QtAxExcelEngine::columnCount()const
{
    return column_count_;
}

//!��ǰ��Sheet����ʼ�����������1,2���ǿգ�û�����ݣ���ô����3
int QtAxExcelEngine::startRow() const
{
	return start_row_;
}

//!��ǰ��Sheet����ʼ�����������1,2,3���ǿգ�û�����ݣ���ô����4
int QtAxExcelEngine::startColumn() const
{
	return start_column_;
}

//
void QtAxExcelEngine::insertSheet(const QString &sheet_name)
{
	work_books_->querySubObject("Add()");
    QAxObject *a = work_books_->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheet_name);
    active_sheet_ = a;

    loadSheet_internal(false);
}


//ȡ���е����ƣ�����27->AA
QString QtAxExcelEngine::columnName(int column_no)
{
    char column_name[64];
    size_t str_len = 0;

    while (column_no > 0)
    {
        int num_data = column_no % 26;
        column_no /= 26;
        if (num_data == 0)
        {
            num_data = 26;
            column_no--;
        }
        //��֪������Բ���
        column_name[str_len] = (char)((num_data - 1) + ('A'));
        str_len++;
    }
    column_name[str_len] = '\0';
    //��ת
    strrev(column_name);

    return column_name;
}