#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QWebEngineView>
#include <QWidget>

class View : public QWebEngineView
{
  Q_OBJECT
public:
  explicit View(QWidget *parent = nullptr);
  void alertMessage(QString);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
private:

  void mouseMoveEvent(QMouseEvent *event) override;
  bool clickedMouse;
  qreal oldPosMouseY;
};

#endif // VIEW_H
