#include <QtWidgets/QApplication>
#include <qthread.h>
#include "canvas.h"
#include "rayTracing.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    CurrentSceneType = SceneType::OneWeekend;

    RayTracing rayTracing;
    
    Canvas canvas;
    canvas.show();

    QThread thread;
    rayTracing.moveToThread(&thread);

    QObject::connect(&thread, &QThread::started, &rayTracing, &RayTracing::slotRender);
    QObject::connect(&rayTracing, &RayTracing::sigRender, &canvas, &Canvas::slotRender);
    QObject::connect(&rayTracing, &RayTracing::sigRenderStart, &canvas, &Canvas::slotRenderStart);
    QObject::connect(&rayTracing, &RayTracing::sigRenderEnd, &canvas, &Canvas::slotRenderEnd);
    QObject::connect(&canvas, &Canvas::sigClose, [&] {
        thread.quit();
        thread.wait();
        });

    thread.start();

    return a.exec();
}
