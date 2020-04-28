#include "view.h"
#include <QApplication>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QWebEngineScript>

View::View(QWidget *parent) : QWebEngineView(parent)
{
  QApplication::instance()->installEventFilter(this);
  setMouseTracking(true);
}

void View::alertMessage(QString a)
{
  QMessageBox p;
  p.setText(a);

  QTimer::singleShot(5000, &p, SLOT(close()));
  p.exec();
}

bool View::eventFilter(QObject *object, QEvent *event)
{
  if (object->parent() == this && event->type() == QEvent::MouseMove) {
     mouseMoveEvent(static_cast<QMouseEvent *>(event));
  }

  return false;
}

// Метод реализует прокрутку курсором
void View::mouseMoveEvent(QMouseEvent *event)
{
  bool clickedMouseNew = event->buttons().testFlag(Qt::MouseButton::LeftButton);
  const qreal & oldPosY = page()->scrollPosition().y();
  qreal posMouseY = event->y();

  if(clickedMouseNew) {
    if(!clickedMouse) oldPosMouseY = posMouseY;

    auto command = QString("window.scrollTo(%1, %2);").arg(0).arg(oldPosY + (oldPosMouseY - posMouseY));
    page()->runJavaScript(command,QWebEngineScript::ApplicationWorld);
  }

  clickedMouse = clickedMouseNew;
}
