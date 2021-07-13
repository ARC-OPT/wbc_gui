#include "WbcCtrlGui.hpp"
#include "ControllerInterface.hpp"
#include "ui_WbcCtrlGui.h"
#include <stdexcept>

WbcCtrlGui::WbcCtrlGui(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget()){

    ui->setupUi(this);
    layout = new QGridLayout();
    ui->widget->setLayout(layout);
    show();
}

WbcCtrlGui::~WbcCtrlGui()
{
    for(ControllerInterfaceMap::iterator it = controller_interfaces.begin(); it != controller_interfaces.end(); ++it)
        delete it->second;
}

void WbcCtrlGui::addControllerInterface( const QString &controller_interface_name,
                                         const QStringList &field_names){

    if(controller_interfaces.count(controller_interface_name) > 0){
        LOG_ERROR("Controller Interface Map already contains an entry with name %s", controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid controller interface name");
    }

    ControllerInterface *ci = new ControllerInterface(this, controller_interface_name, field_names);
    controller_interfaces[controller_interface_name] = ci;
    layout->addWidget(ci);

    // Resize Control GUI widget to fit the new number of controller interfaces
    this->resize(ci->size().width(),ci->size().height()*controller_interfaces.size());
}

QWidget* WbcCtrlGui::getControllerInterface(const QString &controller_interface_name)
{
    if(controller_interfaces.count(controller_interface_name) == 0)
    {
        LOG_ERROR("No such controller interface: %s", controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid control interface name");
    }
    return controller_interfaces[controller_interface_name];
}
