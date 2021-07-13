#ifndef CONTROLLERINTERFACE_HPP
#define CONTROLLERINTERFACE_HPP

#include <QWidget>
#include <map>

#include <QtGui>
#include <QTimer>
#include <QLineEdit>
#include <QDialog>
#include <QString>

#include "ImportExportDialog.hpp"

class VectorXdInterface;
class QGridLayout;
class QVBoxLayout;

namespace Ui{
    class ControllerInterfaceWidget;
}

class ControllerInterface : public QWidget
{
    typedef std::map< QString, VectorXdInterface* > InterfaceMap;


    Q_OBJECT
public:
    explicit ControllerInterface(QWidget* _parent,
                                 const QString &_controller_interface_name,
                                 const QStringList &_field_names);
    ~ControllerInterface();

public slots:
    void updateData( const QString interface_name, const QVector<double> &values );
    QVector<double> query( const QString interface_name );
    void addInputPortInterface( const QString &interface_name);
    void addOutputPortInterface( const QString &interface_name);
    void addConfigInterface( const QString &interface_name);
    bool isHidden();
    void updateState(const QString &state);
    void checkKeepSending(const bool check);

    void onOthersButtonClicked();
    void onSendClicked();
    void onResetClicked();
    void onConfigClicked();
    void onKeepSendingCBToggled(bool checked);
    void onImportConfigButtonClicked();
    void onImportConfig(QString path, QString section_name);
    void onExportConfigButtonClicked();
    void onExportConfig(QString path, QString section_name);
    void openImportDialog(QString current_path);
    void openExportDialog(QString current_path);

signals:
    void send();
    void reset();
    void applyConfig();
    void importConfig(QString path, QString section_name);
    void exportConfig(QString path, QString section_name);
    void getCurrentPath(bool); // bool: isImportDialog
    void showDetails();
    void hideDetails();

private:
    ImportExportDialog *ied;

protected:
    Ui::ControllerInterfaceWidget *ui;
    QStringList field_names;
    QString controller_interface_name;

    InterfaceMap interfaces;
    QTimer* keep_Sending_timer;
};

#endif // CONTROLLERINTERFACE_HPP
