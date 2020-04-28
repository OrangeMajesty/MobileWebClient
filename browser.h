#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QWebEngineView>
#include "view.h"

namespace Ui {
  class Browser;
}

class Browser : public QMainWindow
{
  Q_OBJECT

public:
  explicit Browser(QWidget *parent = nullptr);
  ~Browser();
  QHttpMultiPart *makeParam(QMap<QString, QByteArray>);

private slots:
  void pageLoaded();
  void on_switchConnectButton_clicked();
  void on_requestButton_clicked();

private:
  View* view;
  Ui::Browser *ui;
  bool status;
};

#endif // BROWSER_H
