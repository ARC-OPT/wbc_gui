#include "ImportExportDialog.hpp"
#include "ui_ImportExportDialog.h"

ImportExportDialog::ImportExportDialog(QWidget *parent, bool isImportDialog, const std::string path) :
        QDialog(parent),
        ui(new Ui::ImportExportDialog) {
    ui->setupUi(this);
    ui->lePath->setText(QString(path.c_str()));
    this->setFixedSize(this->geometry().width(), this->geometry().height());
    connect(ui->bnSelectPath, SIGNAL(clicked()), this, SLOT(onButtonSelectPathClicked()));

    std::string windowsTitle = "Configuration";
    if (isImportDialog){
        ui->leConfigSectionName->setPlaceholderText(QString("section1,section2,section3"));
        connect(ui->bnBox, SIGNAL(accepted()), this, SLOT(onButtonImportClicked()));
        windowsTitle = "Import " + windowsTitle;
    } else {
        ui->leConfigSectionName->setPlaceholderText(QString("section1"));
        connect(ui->bnBox, SIGNAL(accepted()), this, SLOT(onButtonExportClicked()));
        windowsTitle = "Export " + windowsTitle;
    }
    this->setWindowTitle(QString(windowsTitle.c_str()));

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

void ImportExportDialog::onButtonImportClicked() {
    if (ui->lePath->text().isEmpty() || ui->leConfigSectionName->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("You have empty fields!"), QMessageBox::Ok);
    } else {
        emit(ImportExportDialog::importConfig(ui->lePath->text(), ui->leConfigSectionName->text()));

    }
}

void ImportExportDialog::onButtonExportClicked() {
    if (ui->lePath->text().isEmpty() || ui->leConfigSectionName->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("You have empty fields!"), QMessageBox::Ok);
    } else {
        emit(ImportExportDialog::exportConfig(ui->lePath->text(), ui->leConfigSectionName->text()));
    }
}
