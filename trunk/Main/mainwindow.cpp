#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

//待完善的功能
// 1、取天气用线程实现
// 2、使用公共网络接口，获取城市列表  (已完成)
// 3、显示温度曲线图
// 4、刷新天气信息

//http://wthrcdn.etouch.cn/weather_mini?city=成都
//http://www.sojson.com/open/api/weather/json.shtml?city=成都

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gIndex = false;

    mNetManager = new QNetworkAccessManager(this);
    mNetRequest = new QNetworkRequest;

    connect(mNetManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(onReplyFinished(QNetworkReply*)));
    connect(&mCtrlPanel ,SIGNAL(sngRefreshWeather(QString)), SLOT(onGetWeather(QString)));

    QTimer::singleShot(1500,this,SLOT(onGetWeather()));

    ui->verticalLayout->addWidget(&mCtrlPanel);
    initCustomPlot();

    setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
    mNetManager->deleteLater();
    if(mNetRequest != NULL)
    {
        delete mNetRequest;
        mNetRequest = NULL;
    }
}

void MainWindow::onGetWeather()
{
    qDebug()<< QTime::currentTime().toString();
    mNetRequest->setUrl(QUrl("http://wthrcdn.etouch.cn/weather_mini?city=成都"));
    mNetRequest->setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    mNetManager->get(*mNetRequest);
}

void MainWindow::onReplyFinished(QNetworkReply *reply)
{
    qDebug()<< QTime::currentTime().toString();

    QByteArray weather = reply->readAll();
    if(!weather.isEmpty())
    {
        qDebug()<< QString::fromUtf8(weather);
        if(QString::fromLatin1(weather).contains("invilad-citykey") ||
                !QString::fromLatin1(weather).contains(QString("city")))
        {
            QMessageBox msgBox;
            msgBox.setText(QString::fromLocal8Bit("城市码错误！"));
            msgBox.exec();
            return ;
        }
        analyWeatherXML(weather); //解析天气信息的数据
    }
    reply->deleteLater();
}

void MainWindow::onGetWeather(QString city)
{
    //响应获取按钮
    if(city.isEmpty())
        return;
    mNetRequest->setUrl(QUrl(QString::fromLocal8Bit("http://wthrcdn.etouch.cn/weather_mini?city=%1").arg(city)));
    mNetRequest->setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    mNetManager->get(*mNetRequest);
}

void MainWindow::analyWeatherXML(QByteArray xml)
{
    if(xml.isEmpty())
        return ;

    qDebug()<< QString::fromUtf8(xml);
    mValues.clear(); //清空历史数据

    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(xml,&err);
    QJsonObject jsonObj= jsonDoc.object().value("data").toObject();

    QJsonObject yesterday = jsonObj.value("yesterday").toObject(); //昨天
    ui->textBrowserYesterday->clear();
    ui->textBrowserYesterday->append(JsonObj2String(yesterday));

    //预测
    QJsonArray forecast = jsonObj.value("forecast").toArray();
    QJsonObject day0 = forecast[0].toObject(); //今天
    ui->textBrowserToday->clear();
    ui->textBrowserToday->append(JsonObj2String(day0));
    ui->today->clear();
    ui->today->append(JsonObj2String(day0));

    QJsonObject day1 = forecast[1].toObject(); //明天
    ui->textBrowser_3->clear();
    ui->textBrowser_3->append(JsonObj2String(day1));

    QJsonObject day2 = forecast[2].toObject();
    ui->textBrowser_4->clear();
    ui->textBrowser_4->append(JsonObj2String(day2));

    QJsonObject day3 = forecast[3].toObject();
    ui->textBrowser_6->clear();
    ui->textBrowser_6->append(JsonObj2String(day3));

    QJsonObject day4 = forecast[4].toObject();
    ui->textBrowser_7->clear();
    ui->textBrowser_7->append(JsonObj2String(day4));

    ui->wendu->setText(jsonObj.value("wendu").toString());
    ui->tips->setText(jsonObj.value("ganmao").toString());
    ui->city->setText(jsonObj.value("city").toString());

    appendData2Plot(mValues);
}

QString MainWindow::JsonObj2String(const QJsonObject jsonObj)
{
    QString weather;
    if(!jsonObj.isEmpty())
    {
        weather += jsonObj.value("date").toString()          + "\n";
        if(!jsonObj.value("fx").toString().isEmpty())
            weather += jsonObj.value("fx").toString()        + "\n";
        else
            weather += jsonObj.value("fengxiang").toString() + "\n";
        weather += jsonObj.value("high").toString()          + "\n";
        weather += jsonObj.value("low").toString()           + "\n";
        weather += jsonObj.value("type").toString();
        QString low = jsonObj.value("low").toString().split(" ").at(1);
        QString tmp = low.remove(low.count() - 1 , 1);
        mValues.append(tmp.toDouble());
    }
    return weather;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    bPressFlag = true;
    dragPosition = event->pos();
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (bPressFlag)
    {
        QPoint relaPos(QCursor::pos() - dragPosition);
        move(relaPos);
    }
    QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    gIndex = !gIndex; //每次翻转一次
    if( mouseEvent->buttons() == Qt::LeftButton) //判断是否左键双击
    {
        if(gIndex & 0x1)
        {
            this->showMaximized();
        }
        else
        {
            this->showNormal();
        }
    }
}

void MainWindow::initCustomPlot()
{
    ui->widget->setBackground(Qt::darkGray);
    ui->widget->setWindowTitle("Plot");
    ui->widget->xAxis->setLabel(QString::fromLocal8Bit("日期"));
    ui->widget->yAxis->setLabel(QString::fromLocal8Bit("气温"));

    ui->widget->xAxis->setRange(0, 7);
    ui->widget->yAxis->setRange(-15, 30);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::blue));
    ui->widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->widget->legend->setVisible(true);
    ui->widget->graph(0)->setName(QString("温度曲线"));
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->widget->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 8));
}

void MainWindow::appendData2Plot(const QVector<double> &values)
{
    if(values.count() == 7)
    {
        QVector<double> keys;
        keys << 0 << 1 << 2<< 3 << 4 << 5 << 6;
        ui->widget->graph(0)->setData(keys , values);
        int min = 0 ;
        int max = 0 ;
        foreach (double d, values)
        {
            if(d > max)
                max = d;
            if(d < min)
                min = d;
        }
        ui->widget->yAxis->setRange(min - 5 , max + 5);
        ui->widget->replot();
    }
}
