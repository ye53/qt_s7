#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtCharts>
#include <QMainWindow>
#include <snap7.h>
#include <QWidget>
#include <QTimer>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

#include "login.h"

QT_BEGIN_NAMESPACE
using namespace QtCharts;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    TS7Client *client=new TS7Client();
    byte buff_M[10] = {0};
    byte buff_V[10] = {0};
    ushort freq;
    QChart* chart ;
    QLineSeries *series;
    quint32 counter;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void slotBtnClear();

    void slotBtnStartAndStop();

    void slotTimeout();


    void on_login_clicked();

    void on_register_2_clicked();

    void on_pushButton_14_clicked();

    void on_FW_run_clicked();

    void on_stop_clicked();

    void on_RW_run_clicked();

    void on_Freq_set_clicked();

private:
    Ui::MainWindow *ui;
    /* 用于模拟生成实时数据的定时器 */
    QTimer* m_timer;

    /* 图表对象 */
    QChart* m_chart;

    /* 横纵坐标轴对象 */
    QValueAxis *m_axisX, *m_axisY;

    /* 曲线图对象 */
    QLineSeries* m_lineSeries;

    /* 横纵坐标最大显示范围 */
    const int AXIS_MAX_X = 5, AXIS_MAX_Y = 20;

    /* 用来记录数据点数 */
    int pointCount = 0;
};
#endif // MAINWINDOW_H
