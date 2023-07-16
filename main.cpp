#include <QCoreApplication>
#include <QSqlDatabase>
#include <QNetworkInterface>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase db_name;
    db_name.close();
    QNetworkInterface i_net;

    return a.exec();
}
