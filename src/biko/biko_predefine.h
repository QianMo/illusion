#ifndef POPULOUS_PREDEFINE_H_
#define POPULOUS_PREDEFINE_H_



// ��WINDOWS�º�POSIX��׼���ݵĺ꣬VS2003���°汾��Σ���û�в��ԣ�2003�Ժ�Windows���ںܶ������ʹ��"_"ǰ׺��
#ifndef _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS  1
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#endif

// ���ڸ���VC++���Ҳ�ǿ��ʹ��_s���͵�API��_CRT_SECURE_NO_DEPRECATE��_CRT_SECURE_NO_WARNINGS���ϰ汾
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS   1
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  1
#endif

#pragma warning ( push )
#pragma warning ( disable : 4091)

// ���¶���FD_SETSIZE����Ҫ��winsock2.hǰ�棬Ҳ��������ע��
#ifndef FD_SETSIZE
#define FD_SETSIZE   1024
#endif

//��Щ�ļ�ǰ׺�Ǵ�д�ģ��������ֵֹģ���Windows���������Ǵ�д�ġ�

#include <winsock2.h>
#include <MSWSock.h>
#include <winerror.h>
#include <windows.h>
#include <winnt.h>
#include <winbase.h>
#include <Psapi.h>
#include <windef.h>
#include <WTypes.h>
#include <process.h>
#include <ws2tcpip.h>
#include <direct.h>
#include <crtdbg.h>
#include <io.h>
#include <share.h>
#include <DbgHelp.h>
#include <intrin.h>

#pragma warning ( pop )

// C ͷ�ļ�
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>

//==================================================================================================
#if defined ZCE_OS_WINDOWS
#pragma warning ( push )
#pragma warning ( disable : 4702)
#pragma warning ( disable : 4267)
#endif

// c++ͷ�ļ�
#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <utility>
#include <iostream>
#include <iomanip>
#include <functional>
#include <iomanip>
#include <typeinfo>
#include <algorithm>
#include <memory>
#include <limits>

//hash_map,hash_set��ͷ�ļ���������Ҫ�鷳һ��

//��VS2008�Ժ󣬲���unordered_map��unordered_set����������֮ǰ���������stlport��
//��Ȼ����stlport������ǿ��΢���Դ��������������汾Ҳ��������stlport
#if defined ZCE_OS_WINDOWS && !defined _STLP_CONFIX_H && defined (_MSC_VER) && (_MSC_VER <= 1400)
#error " Please use stlport ,in Visual studio 2005, have not unordered_map and unordered_set ."
#endif

//��VC++2008�汾,VC++2005+STLport��GCC 4.6�汾�Լ�����İ汾��unordered_map�����ֿռ���std::tr1
//��VC++2008�汾��ǰ(����),����ʵ��STLport
//��VC++2008�汾�󣬿��Կ����Ƿ�ʵ��STLport,���_STLP_CONFIX_H �������ˣ�����Ϊ����ʹ��
#if (defined ZCE_OS_LINUX && (_GCC_VER <= 40600)) \
    || ( defined ZCE_OS_WINDOWS && (_MSC_VER <= 1400) ) \
    || ( defined ZCE_OS_WINDOWS && (_MSC_VER > 1400) && defined _STLP_CONFIX_H)
#include <unordered_map>
#include <unordered_set>
using std::tr1::unordered_map;
using std::tr1::unordered_set;

//����İ汾����ֱ������std�����ֿռ�
#else
#include <unordered_set>
#include <unordered_map>
using std::unordered_map;
using std::unordered_set;
#endif
//����İ汾��ʵ��֧��hash_map��hash_set��ͷ�ļ��ģ����ҷ���֧����,�Ǹ�Ҫ��һ����롣

#if defined ZCE_OS_WINDOWS
#pragma warning ( pop )
#endif



#pragma warning ( push )
#pragma warning ( disable : 4127)

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <ActiveQt/QAxBase>
#include <ActiveQt/QAxObject>

#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtCore/QVariant>
#include <QtCore/QDir> 

#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidgetItem>

#pragma warning ( pop )




#if defined (ZCE_OS_WINDOWS)
#pragma warning ( push )
#pragma warning ( disable : 4512)
#pragma warning ( disable : 4100)
#elif defined (ZCE_OS_LINUX)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/text_format.h>

#if defined (ZCE_OS_WINDOWS)
#pragma warning ( pop )
#elif defined (ZCE_OS_LINUX)
#pragma GCC diagnostic pop
#endif

#endif //


