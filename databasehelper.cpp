#include "databasehelper.h"
#include <iostream>
using namespace std;

databasehelper::databasehelper()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("182.92.66.200");
    db.setPort(3306);
    db.setDatabaseName("llk");
    db.setUserName("guest");
    db.setPassword("123456");

    bool ok = db.open();
    if(!ok)
    {
        cout << "���ݿ�����ʧ�ܣ�" << endl;
    }else
    {
        cout << "���ݿ����ӳɹ���" << endl;
    }
}
