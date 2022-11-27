#ifndef SERIALLINK_H
#define SERIALLINK_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>

class seriallink : public QObject
{
    Q_OBJECT
public:
    explicit seriallink(QObject *parent = nullptr);
    ~seriallink();

    void openConnection();
    void closeConnection();
    void write(const char* messageToWrite);
    bool isOpen();
    bool isWritable();

private slots:
    void newData();

signals:
    void gotNewData(QByteArray data);

private:
    QSerialPort serial;

};

#endif // SERIALLINK_H
