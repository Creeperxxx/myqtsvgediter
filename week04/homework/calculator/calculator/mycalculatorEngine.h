#pragma once
#ifndef MYCALCULATORENGINE_H
#define MYCALCULATORENGINE_H
#include <QString>
#include <QStack>
#include <QQueue>
#include <QDebug>
class mycalculatorEngine
{
private:
    QQueue<QString> Split(const QString& exp);          //分离前缀
    QQueue<QString> Transfer(QQueue<QString>& exp);     //将中缀队列转换为后缀队列
    QString Calculate(QQueue<QString>& exp);            //将后缀队列计算出结果

    QString Calculate(QString& l, QString& op, QString& r);
    QString ValidNum(QString str);

public:
    mycalculatorEngine();
    QString Result(const QString& exp);
};

#endif // MYCALCULATORENGINE_H
