#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <math.h>
#include <QTime>
#include <QtNetwork/QTcpSocket>
#include <windows.h>
#include <time.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(RCcar, &QTcpSocket::connected, this, [this](){
        ui->status->setStyleSheet("background-color:green;");
        ui->loginpress->setText("Disconnect");
        ui->Thottle->setEnabled(1);
        ui->Turn->setEnabled(1);
        ui->lghts->setEnabled(1);
        ui->movement->setEnabled(1);
    });

    connect(RCcar, &QTcpSocket::disconnected, this, [this](){
        ui->status->setStyleSheet("background-color:red;");
        ui->loginpress->setText("Connect");
        conStatus = 1;
        ui->Thottle->setEnabled(0);
        ui->Turn->setEnabled(0);
        ui->lghts->setEnabled(0);
        ui->movement->setEnabled(0);
        ui->movement->setChecked(0);
        ui->lghts->setChecked(0);
        ui->Thottle->setValue(0);
        ui->Turn->setValue(90);
    });

}



MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::updateList(MainWindow::signalDirection variant)
{
    ui->logInfo->insertRow(row);
    QTableWidgetItem *row0 = new QTableWidgetItem(QTime::currentTime().toString("HH:mm:ss"));
    QTableWidgetItem *row1;
    QTableWidgetItem *row2;
    ui->FrameView->setText(TXFrame);
    switch(variant)
    {
        case 0:
        {
            row2 = new QTableWidgetItem(MainWindow::TXFrame);
            row1 = new QTableWidgetItem("I");
            break;
        }
        case 1:
        {
            row2 = new QTableWidgetItem(MainWindow::TXFrame);
            row1 = new QTableWidgetItem("O");
            break;
        }
        case 2:
        {
            row2 = new QTableWidgetItem("Connection Status Changed");
            row1 = new QTableWidgetItem("G");
            break;
        }
    }
    ui->logInfo->setItem(row,0,row0);
    ui->logInfo->setItem(row,1,row1);
    ui->logInfo->setItem(row,2,row2);
    row++;
    RCcar->write(TXFrame.toUtf8());
    Sleep(20);
}

void MainWindow::calculateString(char v, int val)
{
    std::string tmpS;
    int tmpVal = val;
    for(int i =2; i >=0; i--)
    {
        tmpS[2-i] = (tmpVal/pow(10,i))+48;
        tmpVal = tmpVal - (tmpS[2-i]-48)*pow(10,i);
    }

    int start, stop;
    switch(v)
    {
        case 'S': start =5; stop =8; break;
        case 'A': start =9; stop =12; break;
    }
    for(int i = start; i < stop; i++) TXFrame[i] = tmpS[i-start];
}

void MainWindow::on_loginpress_clicked()
{
    try
    {
        if(!(RCcar->state() == QAbstractSocket::ConnectedState))
        {
            std::string portS = ui->portIn->text().toStdString();
            int port = std::stoi(portS);
            RCcar->connectToHost(ui->ipIn->text(),port);
        }
        else
        {
            RCcar->disconnectFromHost();
        }
    }
    catch (...)
    {
        ui->FrameView->setText("Error!");
    }

}


void MainWindow::on_Thottle_valueChanged(int value)
{
    calculateString('S',value);
    updateList(OUTPUT);
}


void MainWindow::on_Turn_valueChanged(int value)
{
    calculateString('A',value);
    updateList(OUTPUT);
}


void MainWindow::on_lghts_stateChanged(int arg1)
{
    if(ui->lghts->isChecked()) TXFrame[3] = '1';
    else TXFrame[3] = '0';
    updateList(OUTPUT);
}


void MainWindow::on_movement_stateChanged(int arg1)
{
    ui->Thottle->setValue(0);
    if(ui->movement->isChecked()) TXFrame[1] = '1';
    else TXFrame[1] = '0';

    TXFrame[5] = '0'; TXFrame[6] = '0'; TXFrame[7] = '0';
    updateList(OUTPUT);
}


