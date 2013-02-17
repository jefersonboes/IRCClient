#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

//for mingw and qt compliance
extern "C" void _Unwind_Resume()
{

}

extern "C" void __gxx_personality_v0()
{

}

extern "C" int __mingwthr_key_dtor (unsigned long, void (*) (void *))
{
    return 0;
}
