#include "fileinitial.h"
#include "ui_fileinitial.h"

FileInitial::FileInitial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInitial)
{
    ui->setupUi(this);
}

FileInitial::~FileInitial()
{
    delete ui;
}

void FileInitial::on_file_clicked()
{
    files=QFileDialog::getOpenFileUrls(nullptr,QString(),QUrl(),"*.txt");                                       //open dialog to select multiple files with ".txt" extension
    if (files.length()>0){
        QMessageBox::information(this,"File info","Number of files selected: "+QString::number(files.length()));    //Notify user about number of files selected
        ui->plot->setEnabled(true);
    }
}

void FileInitial::on_plot_clicked()
{
    emit fileConfig(files);     //pass the files selected
    this->close();              //close this window
}
