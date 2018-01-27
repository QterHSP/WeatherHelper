#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ctrlpanel.h"
#include "qcustomplot/qcustomplot.h"

namespace Ui
{
class MainWindow;
}

class CtrlPanel;
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onGetWeather();
    void onReplyFinished(QNetworkReply *reply);
    void onGetWeather(QString city);


private:
    void analyWeatherXML(QByteArray xml);
    QString JsonObj2String(const QJsonObject jsonObj);

    void mousePressEvent (QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);

    void initCustomPlot();

    void appendData2Plot(const QVector<double> &values);


private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *mNetManager ;
    QNetworkRequest *mNetRequest;

    CtrlPanel mCtrlPanel;

    bool bPressFlag;
    QPoint dragPosition;

    bool gIndex;
    QVector<double> mValues;
};

#endif // MAINWINDOW_H
