#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    process(NULL)
{
    ui->setupUi(this);
    this->statusBar()->showMessage("Ready.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_runButton_clicked()
{
    this->ui->runButton->setEnabled(false);

    QString executable = this->ui->binaryFileField->text();
    QString dbName = this->ui->dbUrlField->text();
    QString pop3Server = this->ui->pop3ServerField->text();
    QString email = this->ui->emailField->text();
    QString password = this->ui->passwordField->text();

    this->process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput, [this](){
        QString output = this->process->readAllStandardOutput();
        this->ui->outputTextEdit->moveCursor(QTextCursor::End);
        this->ui->outputTextEdit->insertPlainText(output);
        this->ui->outputTextEdit->moveCursor(QTextCursor::End);
    });

    connect(process, &QProcess::readyReadStandardError, [this](){
        QString output = this->process->readAllStandardError();
        this->ui->outputTextEdit->moveCursor(QTextCursor::End);
        this->ui->outputTextEdit->insertPlainText(output);
        this->ui->outputTextEdit->moveCursor(QTextCursor::End);
    });

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_process_finished(int,QProcess::ExitStatus)));

    this->statusBar()->showMessage("Running...");
    this->process->start(executable, QStringList() << dbName << pop3Server << email << password);
}

void MainWindow::on_binaryFileBrowseButton_clicked()
{
    QFileDialog dialog;
    dialog.setFilter(QDir::Executable);
    QString filename = dialog.getOpenFileName(this, tr("Executable Files"), QDir::homePath());
    this->ui->binaryFileField->setText(filename);
}

void MainWindow::on_process_finished(int code, QProcess::ExitStatus status)
{
    QString exitMessage;
    switch(status) {
    case QProcess::NormalExit:
        exitMessage = "Success.";
        break;
    case QProcess::CrashExit:
        exitMessage = "Failed.";
        break;
    }

    this->ui->outputTextEdit->moveCursor(QTextCursor::End);
    this->ui->outputTextEdit->insertPlainText(exitMessage);
    this->ui->outputTextEdit->moveCursor(QTextCursor::End);
    this->ui->runButton->setEnabled(true);
    this->statusBar()->showMessage("Done.");

    delete process;
}
