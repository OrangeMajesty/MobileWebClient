#include "browser.h"
#include "ui_browser.h"
#include <QtWebEngine/QtWebEngine>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QWebEnginePage>
#include <QScroller>
#include <QWebEngineView>
#include <QEventLoop>
#include <QNetworkReply>

Browser::Browser(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Browser)
{
  ui->setupUi(this);
  status = false;

  QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Linux; Android 7.0; SM-G930V Build/NRD90M) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.125 Mobile Safari/537.36");

  view = new View(ui->view);
  view->setMinimumSize(320, 640);
  view->show();

  connect(view, &QWebEngineView::loadFinished, this, &Browser::pageLoaded);
}

Browser::~Browser()
{
  delete ui;
}

QHttpMultiPart *Browser::makeParam(QMap<QString, QByteArray> list)
{
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QMapIterator<QString, QByteArray> iter(list);

  while(iter.hasNext())
  {
      iter.next();
      QHttpPart textParam;
      QVariant paramKey(QString("form-data; name=\"%1\"").arg(iter.key()));
      textParam.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant());
      textParam.setBody(iter.value());
      multiPart->append(textParam);
  }

  return multiPart;

}

void Browser::pageLoaded()
{
  ui->statusbar->showMessage("Page loaded");
}

void Browser::on_switchConnectButton_clicked()
{

  if(!status) {
    view->load(QUrl("http://skydns.ru/home"));
    ui->statusbar->showMessage("Loading page http://skydns.ru/home");
  } else {
    view->stop();
    ui->statusbar->showMessage("Connecton closed");
  }

  status = !status;
}

void Browser::on_requestButton_clicked()
{
  QMap<QString, QByteArray> param;
  param.insert("param1", "1");
  param.insert("param2", "2");

  QHttpMultiPart *multiPart = makeParam(param);

  QUrl url("http://127.0.0.1:8000/api");
  QNetworkRequest request(url);

  QNetworkAccessManager manager;

  // Синхронизация запроса
  QEventLoop requestWait;

  connect(&manager, &QNetworkAccessManager::finished, &requestWait, &QEventLoop::quit);

  QNetworkReply *reply = manager.post(request, multiPart);

  requestWait.exec();

  if(!reply->error()) {
      auto ret = reply->readAll();
      view->alertMessage(ret);
  } else {
      view->alertMessage("Error send request");
      qDebug() << "Error send request" << url;
  }

  delete multiPart;

}
