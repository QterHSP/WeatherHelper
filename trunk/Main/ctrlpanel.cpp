#include "ctrlpanel.h"
#include "ui_ctrlpanel.h"
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QVariant>

CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent), ui(new Ui::CtrlPanel)
{
    ui->setupUi(this);

    mNetManager = new QNetworkAccessManager(this);
    mNetRequest = new QNetworkRequest;

    connect(ui->btnRefresh , SIGNAL(clicked(bool)) , SLOT(onBtnRefreshWeather()));
    connect(mNetManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(onReplyFinished(QNetworkReply*)));

    QTimer::singleShot(1000 ,this ,  SLOT(onSendRequest()));
}

CtrlPanel::~CtrlPanel()
{
    delete ui;
}

void CtrlPanel::onSendRequest()
{
    mNetRequest->setUrl(QUrl("http://www.weather.com.cn/data/city3jdata/china.html"));
    mNetRequest->setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    mNetManager->get(*mNetRequest);
}

void CtrlPanel::onReplyFinished(QNetworkReply *reply)
{
    QByteArray weather = reply->readAll();
    if(!weather.isEmpty())
        analyCitysXML(weather); //解析天气信息的数据
    reply->deleteLater();
}

void CtrlPanel::onBtnRefreshWeather()
{
    if(!ui->comboBox_Citys->currentText().isEmpty())
        emit sngRefreshWeather(ui->comboBox_Citys->currentText());
}

void CtrlPanel::analyCitysXML(QByteArray xml)
{
    if(xml.isEmpty())
        return ;

    QJsonParseError err;
    QStringList citys;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(xml,&err);
    QVariantMap variantMap = jsonDoc.object().toVariantMap();
    if(variantMap.count() > 0)
    {
        foreach (auto c, variantMap)
        {
            citys.append(c.toString());
        }
        setCitys2UI(citys);
    }
}

void CtrlPanel::setCitys2UI(const QStringList citys)
{
    ui->comboBox_Citys->clear();
    ui->comboBox_Citys->addItems(citys);
}
