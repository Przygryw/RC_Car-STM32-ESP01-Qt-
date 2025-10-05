 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum signalDirection
    {
        INPUT,
        OUTPUT,
        GENERAL,
    };

    int speed;
    int angle;
    bool reverse;
    bool lights;
    int row = 0;
    QString TXFrame = "R0L0S000A000/";
    QTcpSocket* RCcar = new QTcpSocket(this);
    bool conStatus = 1;
    void frameModify();
    void updateList(signalDirection variant);
    void calculateString(char v, int val);

private slots:
    void on_loginpress_clicked();

    void on_Thottle_valueChanged(int value);

    void on_Turn_valueChanged(int value);

    void on_lghts_stateChanged(int arg1);

    void on_movement_stateChanged(int arg1);


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
