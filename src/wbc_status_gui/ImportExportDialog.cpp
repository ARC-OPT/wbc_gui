#include "ImportExportDialog.hpp"
#include "ui_ImportExportDialog.h"

ImportExportDialog::ImportExportDialog(QWidget *parent, bool is_import_dialog, const std::string path) :
        QDialog(parent),
        ui(new Ui::ImportExportDialog),
        is_import_dialog(is_import_dialog){

    ui->setupUi(this);
    ui->lePath->setText(QString(path.c_str()));
    this->setFixedSize(this->geometry().width(), this->geometry().height());
    connect(ui->bnSelectPath, SIGNAL(clicked()), this, SLOT(onButtonSelectPathClicked()));

    std::string window_title = "Configuration";
    connect(ui->bnBox, SIGNAL(accepted()), this, SLOT(onAcceptedClicked()));

    if (is_import_dialog){
        ui->leConfigSectionName->setText(QString("section1,section2,section3"));
        window_title = "Import " + window_title;
    }
    else {
        ui->leConfigSectionName->setText(QString("section1"));
        window_title = "Export " + window_title;
    }
    this->setWindowTitle(QString(window_title.c_str()));

    connect(ui->bnBox, SIGNAL(rejected()), this, SLOT(reject()));
    show();
}

ImportExportDialog::~ImportExportDialog() {
    delete ui;
}

void ImportExportDialog::onButtonSelectPathClicked() {
    // TODO handle new files and existing files - handles only existing files with a workaround for new files
    QString fileName = QFileDialog::getOpenFileName(this, "Select configuration file", "", "YML (*.yml)");
    ui->lePath->setText(fileName);
}

void ImportExportDialog::onAcceptedClicked() {
    if (ui->lePath->text().isEmpty() || ui->leConfigSectionName->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("You have empty fields!"), QMessageBox::Ok);
    } else {
        if(is_import_dialog)
            emit(ImportExportDialog::importConfig(ui->lePath->text(), ui->leConfigSectionName->text()));
        else
            emit(ImportExportDialog::exportConfig(ui->lePath->text(), ui->leConfigSectionName->text()));
    }
}
