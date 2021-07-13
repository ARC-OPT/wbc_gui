#ifndef IMPORTEXPORTDIALOG_HPP
#define IMPORTEXPORTDIALOG_HPP

#include <QDialog>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
    class ImportExportDialog;
}

class ImportExportDialog : public QDialog {
Q_OBJECT

public:
    //explicit ImportExportDialog(QWidget *parent = 0);
    ImportExportDialog(QWidget *parent = 0, bool isImportDialog = true, const std::string path = "/");

    virtual ~ImportExportDialog();


public slots:
    void onButtonSelectPathClicked();
    void onButtonImportClicked();
    void onButtonExportClicked();

signals:
    void importConfig(QString path, QString section_name);
    void exportConfig(QString path, QString section_name);

private:
    Ui::ImportExportDialog *ui;
};


#endif // IMPORTEXPORTDIALOG_HPP
