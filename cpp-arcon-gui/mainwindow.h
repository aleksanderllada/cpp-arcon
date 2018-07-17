#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_runButton_clicked();
    void on_binaryFileBrowseButton_clicked();
    void on_process_finished(int code, QProcess::ExitStatus status);

private:
    Ui::MainWindow *ui;
    QProcess* process;
};

#endif // MAINWINDOW_H
