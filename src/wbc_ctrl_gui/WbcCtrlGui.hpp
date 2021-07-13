#ifndef WBCCTRLGUI_HPP
#define WBCCTRLGUI_HPP

#include "base-logging/Logging.hpp"
#include <QtGui>
#include <QWidget>

namespace Ui {
    class MainWidget;
}

class QGridLayout;
class ControllerInterface;

typedef std::map<QString, ControllerInterface*> ControllerInterfaceMap;

class WbcCtrlGui : public QWidget
{
    Q_OBJECT
public:
    WbcCtrlGui(QWidget *parent = 0);
    virtual ~WbcCtrlGui();

public slots:
    void addControllerInterface( const QString &controller_interface_name, const QStringList &field_names);
    QWidget* getControllerInterface(const QString &controller_interface_name);

private:
    Ui::MainWidget* ui;
    QGridLayout* layout;
    ControllerInterfaceMap controller_interfaces;

};

#endif /* WBCCTRLGUI_HPP */
