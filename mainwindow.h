#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
using namespace std;
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

private slots:
    void on_pushButton_delete_line_clicked();

    void on_pushButton_add_lines_ready_clicked();

    void on_pushButton_stage_1_clicked();

    void on_pushButton_stage_2_clicked();

    void on_pushButton_stage_3_clicked();

    bool checkOver();

    bool checkOver2();

    bool checkOver3();

    void step2();

    void on_pushButton_gen_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
