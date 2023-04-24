#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));

    QObject::connect(ui->btnClear, SIGNAL(clicked(bool)), this, SLOT(slotBtnClear()));
    QObject::connect(ui->btnStartAndStop, SIGNAL(clicked(bool)), this, SLOT(slotBtnStartAndStop()));


    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_axisX->setMin(0);
    m_axisY->setMin(0);
    m_axisX->setMax(AXIS_MAX_X);
    m_axisY->setMax(AXIS_MAX_Y);

    m_lineSeries = new QLineSeries();                             // 创建曲线绘制对象
    m_lineSeries->setPointsVisible(1);                            // 设置数据点可见
    m_lineSeries->setName("频率曲线");

    m_chart = new QChart();                                        // 创建图表对象
    m_chart->addAxis(m_axisX, Qt::AlignLeft);                      // 将X轴添加到图表上
    m_chart->addAxis(m_axisY, Qt::AlignBottom);                    // 将Y轴添加到图表上
    m_chart->addSeries(m_lineSeries);                              // 将曲线对象添加到图表上
    m_chart->setAnimationOptions(QChart::SeriesAnimations);        // 动画：能使曲线绘制显示的更平滑，过渡效果更好看

    m_lineSeries->attachAxis(m_axisX);                             // 曲线对象关联上X轴，此步骤必须在m_chart->addSeries之后
    m_lineSeries->attachAxis(m_axisY);                             // 曲线对象关联上Y轴，此步骤必须在m_chart->addSeries之后

    ui->graphicsView->setChart(m_chart);                           // 将图表对象设置到graphicsView上进行显示
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);       // 设置渲染：抗锯齿，如果不设置那么曲线就显得不平滑
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString ip=ui->lineEdit->text();
    client->ConnectTo(ip.toStdString().c_str(),0,1);
    if(client->Connected())
    {
        QMessageBox::about(this, tr("提示"), tr("成功连接"));

        ui->pushButton->setEnabled(0);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(client->Connected())
    {
        client->Disconnect();
        QMessageBox::about(this, tr("提示"), tr("断开连接"));
        ui->pushButton->setEnabled(1);
    }
}



void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::slotBtnClear()
{
    m_lineSeries->clear();
    m_chart->axisX()->setMin(0);
    m_chart->axisX()->setMax(AXIS_MAX_X);
    pointCount = 0;
}

void MainWindow::slotBtnStartAndStop()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
        ui->btnStartAndStop->setText("启动定时器");
    }else
    {
        pointCount = 0;
        m_timer->start(1000);
        ui->btnStartAndStop->setText("停止定时器");
    }
}

void MainWindow::slotTimeout()
{
    byte value[10];
    buff_M[0] = (byte)(0xff & 1);
    client->MBWrite(2,1,&buff_M[0]);
    client->DBRead(1,300,2,&value);
    freq= (value[0]<<8) + value[1];
    //value[1] = (byte)((0xff00 & i) >> 8);
    qDebug()<<freq;
    if(pointCount > AXIS_MAX_X)
    {
        m_lineSeries->remove(0);
        m_chart->axisX()->setMin(pointCount - AXIS_MAX_X);
        m_chart->axisX()->setMax(pointCount);                    // 更新X轴范围
    }
        m_chart->axisY()->setMin(0);
        m_chart->axisY()->setMax(50);                    // 更新X轴范围
    m_lineSeries->append(QPointF(pointCount, freq / 100));
    pointCount++;
}

void MainWindow::on_login_clicked()
{
    Login *login =new Login();
    login->show();
}

void MainWindow::on_register_2_clicked()
{

}

void MainWindow::on_pushButton_14_clicked()
{

}

void MainWindow::on_FW_run_clicked()
{
    if(client->Connected())
    {
        buff_M[0] = (byte)(0xff & 2);
        buff_V[0] = (byte)(0xff & 1);
        client->DBWrite(1,305,1,&buff_V[0]);
        client->MBWrite(2,1,&buff_M[0]);
    }
}

void MainWindow::on_stop_clicked()
{
    if(client->Connected())
    {
        buff_M[0] = (byte)(0xff & 8);
        buff_V[0] = (byte)(0xff & 5);
        client->DBWrite(1,305,1,&buff_V[0]);
        client->MBWrite(2,1,&buff_M[0]);
    }
}

void MainWindow::on_RW_run_clicked()
{
    if(client->Connected())
    {
        buff_M[0] = (byte)(0xff & 4);
        buff_V[0] = (byte)(0xff & 2);
        client->DBWrite(1,305,1,&buff_V[0]);
        client->MBWrite(2,1,&buff_M[0]);
    }
}

void MainWindow::on_Freq_set_clicked()
{
    if(client->Connected())
    {
        QString edit_frequency = ui->lineEdit_2->text();
        int edit_freq = edit_frequency.toInt();
        byte value[4];
        memset(value,0,sizeof (byte)*4);
        value[1]=(byte)(edit_freq & 0xff);
        value[0] = (byte)((edit_freq & 0xff00) >> 8);
        //value[] = (byte) ((0xff0000 & freq) >> 16);
        //value[] = (byte) ((0xff000000 & freq) >> 24);
        client ->DBWrite(1,310,2,&value);

        buff_M[0] = (byte)(0xff & 16);
        client->MBWrite(2,1,&buff_M[0]);

    }
}
