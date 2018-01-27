#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

namespace Ui
{
class CtrlPanel;
}

class CtrlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlPanel(QWidget *parent = 0);
    ~CtrlPanel();

private slots:
    void onSendRequest();
    void onReplyFinished(QNetworkReply *reply);
    void onBtnRefreshWeather();

private:
    void analyCitysXML(QByteArray xml);
    void setCitys2UI(const QStringList citys);

signals:
    void sngRefreshWeather(QString citys);

private:
    Ui::CtrlPanel *ui;

    QNetworkAccessManager *mNetManager ;
    QNetworkRequest *mNetRequest;
};

#endif // CTRLPANEL_H
