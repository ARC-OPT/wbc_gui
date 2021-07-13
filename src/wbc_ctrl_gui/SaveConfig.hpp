#ifndef LOAD_CONFIG_HPP
#define LOAD_CONFIG_HPP

#include <QWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QTime>
#include <QDate>
#include "ui_SaveConfig.h"
#include "ControllerInterface.hpp"

namespace Ui{
    class SaveConfigWidget;
}

typedef std::map<std::string, ControllerInterface*> controllerInterfaceMap;

class SaveConfig : public QWidget{
    Q_OBJECT
public:
    explicit SaveConfig(QWidget* parent = 0);
    virtual ~SaveConfig();
    void add_controller(std::string name, ControllerInterface* controller);

public slots:
    void onSaveClicked();
    void onSelectAllClicked();
    void onSelectFileClicked();

signals:
    void saveSignal(QString controllerInterfaceName, QString filename);

private:
    Ui::SaveConfigWidget *ui;
    controllerInterfaceMap controllers;
    bool select_all;
};

#endif
