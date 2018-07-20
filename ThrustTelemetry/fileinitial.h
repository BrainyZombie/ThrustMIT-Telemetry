#ifndef FILEINITIAL_H
#define FILEINITIAL_H

#include <QDialog>
#include "QFileDialog"
#include <QMessageBox>

namespace Ui {
class FileInitial;
}

class FileInitial : public QDialog
{
    Q_OBJECT

public:
    explicit FileInitial(QWidget *parent = 0);
    ~FileInitial();

private slots:
    void on_file_clicked();

    void on_plot_clicked();

private:
    Ui::FileInitial *ui;

    QList<QUrl> files;              //list of files selected stored here

signals:
    void fileConfig(QList<QUrl>);   //Passing list of files selected
};

#endif // FILEINITIAL_H
