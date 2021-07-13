#include "ControllerInterface.hpp"
#include "VectorXdInterface.hpp"
#include "helpers.hpp"

#include <base-logging/Logging.hpp>
#include <ui_ControllerInterface.h>

ControllerInterface::ControllerInterface( QWidget *_parent,
                                          const QString &_controller_interface_name,
                                          const QStringList &_field_names) :
    QWidget(_parent),
    ui(new Ui::ControllerInterfaceWidget),
    field_names(_field_names),
    controller_interface_name(_controller_interface_name),
    keep_Sending_timer(new QTimer(this))

{
    if(_field_names.size() == 0)
        throw std::runtime_error("Controller interface: " + _controller_interface_name.toStdString() + ": Size of field names should be > 0");

    ui->setupUi(this);

    // Set controller name
    ui->groupBox->setTitle(controller_interface_name);

    //Initially hide controls display
    ui->controlsWidget->setHidden(true);

    // Add field names to controls layout i++)
    for(int i = 0; i < field_names.size(); i++){

        QLabel* label = createDefaultLabel(field_names[i], true);
        ui->controls_layout->addWidget(label, 0, i);
        ui->controls_layout->setAlignment(label, Qt::AlignHCenter | Qt::AlignTop);

        label = createDefaultLabel(field_names[i], true);
        ui->config_layout->addWidget(label, 0, i);
        ui->config_layout->setAlignment(label, Qt::AlignHCenter | Qt::AlignTop);
    }

    // Add "all" label
    QLabel* label = createDefaultLabel("all", true);
    ui->config_layout->addWidget(label, 0, field_names.size());
    ui->config_layout->setAlignment(label, Qt::AlignHCenter | Qt::AlignTop);

    // Connect buttons
    connect(ui->othersButton,  SIGNAL(clicked()),     this, SLOT(onOthersButtonClicked()));
    connect(ui->sendButton,    SIGNAL(clicked()),     this, SLOT(onSendClicked()));
    connect(ui->configButton,  SIGNAL(clicked()),     this, SLOT(onConfigClicked()));
    connect(ui->keepSendingCB, SIGNAL(toggled(bool)), this, SLOT(onKeepSendingCBToggled(bool)));
    connect(ui->resetButton,   SIGNAL(clicked()),     this, SLOT(onResetClicked()));

    connect(ui->bnImport,      SIGNAL(clicked()),     this, SLOT(onImportConfigButtonClicked()));
    connect(ui->bnExport,      SIGNAL(clicked()),     this, SLOT(onExportConfigButtonClicked()));
}

ControllerInterface::~ControllerInterface(){
    for (InterfaceMap::iterator it = interfaces.begin(); it != interfaces.end(); it++)
        delete it->second;

    if (ied != NULL) {
        delete ied;
    }
}

void  ControllerInterface::checkKeepSending(const bool check){
    if(check)
        ui->keepSendingCB->setCheckState(Qt::CheckState::Checked);
    else
        ui->keepSendingCB->setCheckState(Qt::CheckState::Unchecked);
}

void ControllerInterface::addInputPortInterface(const QString& interface_name){

    if(interfaces.count(interface_name) > 0){
        LOG_ERROR("Interface with name %s already exists in controller interface %s",
                  interface_name.toStdString().c_str(), controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid interface name");
    }

    int row = ui->controls_layout->rowCount();

    interfaces[interface_name] = new InputPortInterface(this, field_names.size(), ui->controls_layout);

    QLabel* label = createDefaultLabel(interface_name, true);
    ui->controls_names_layout->addWidget(label, row, 0);
    ui->controls_names_layout->setAlignment(label, Qt::AlignRight | Qt::AlignTop);
}

void ControllerInterface::addOutputPortInterface(const QString &interface_name){

    if(interfaces.count(interface_name) > 0){
        LOG_ERROR("Interface with name %s already exists in controller interface %s",
                  interface_name.toStdString().c_str(), controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid interface name");
    }

    int row = ui->controls_layout->rowCount();

    interfaces[interface_name] = new OutputPortInterface(this, field_names.size(), ui->controls_layout);

    QLabel* label = createDefaultLabel(interface_name, true);
    ui->controls_names_layout->addWidget(label, row, 0);
    ui->controls_names_layout->setAlignment(label, Qt::AlignRight | Qt::AlignTop);
}

void ControllerInterface::addConfigInterface( const QString &interface_name){

    if(interfaces.count(interface_name) > 0){
        LOG_ERROR("Interface with name %s already exists in controller interface %s",
                  interface_name.toStdString().c_str(), controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid interface name");
    }

    int row = ui->config_layout->rowCount();

    interfaces[interface_name] = new ConfigInterface(this, field_names.size(), ui->config_layout);

    QLabel* label = createDefaultLabel(interface_name, true);
    ui->config_names_layout->addWidget(label, row, 0);
    ui->config_names_layout->setAlignment(label, Qt::AlignRight | Qt::AlignTop);
}

bool ControllerInterface::isHidden(){
    return ui->controlsWidget->isHidden();
}

void ControllerInterface::updateState(const QString &state){

    QString style_sheet;
    if(state == "RUNNING")
        style_sheet = "QLabel {\nborder: 1px solid lightgrey;\nborder-radius: 5px;\nbackground: rgba(0, 255, 0, 255)\n}\n";
    else
        style_sheet = "QLabel {\nborder: 1px solid lightgrey;\nborder-radius: 5px;\nbackground: rgba(255, 0, 0, 255)\n}\n";

    ui->statusLabel->setStyleSheet(style_sheet);
    ui->statusLabel->setText(state);
}

void ControllerInterface::onKeepSendingCBToggled(bool checked){
    if(checked){
        ui->sendButton->setDisabled(true);
        connect(keep_Sending_timer, SIGNAL(timeout()), this, SLOT(onSendClicked()));
        keep_Sending_timer->start(100);
    }
    else{
        keep_Sending_timer->stop();
        disconnect(keep_Sending_timer, SIGNAL(timeout()), this, SLOT(onSendClicked()));
        ui->sendButton->setDisabled(false);
    }
}

void ControllerInterface::updateData(const QString interface_name, const QVector<double> &values ){

    if(values.size() != field_names.size()){
        LOG_ERROR("Updating controller %s, interface %s: Input vector has size %i but number of fields is %i",
                  controller_interface_name.toStdString().c_str(),
                  interface_name.toStdString().c_str(),
                  values.size(), field_names.size());
        throw std::invalid_argument("Invalid input data size");
    }

    if(interfaces.count(interface_name) == 0){
        LOG_ERROR("updateData: No interface %s in controller %s", interface_name.toStdString().c_str(), controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid interface name");
    }
    interfaces[interface_name]->update(values);
}

QVector<double> ControllerInterface::query(const QString interface_name){

    if(interfaces.count(interface_name) == 0){
        LOG_ERROR("query: No interface %s in controller %s", interface_name.toStdString().c_str(), controller_interface_name.toStdString().c_str());
        throw std::invalid_argument("Invalid interface name");
    }
    return interfaces[interface_name]->query();
}


void ControllerInterface::onOthersButtonClicked(){
    ui->controlsWidget->setHidden(!ui->controlsWidget->isHidden());
    if(ui->controlsWidget->isHidden())
        hideDetails();
    else
        showDetails();
}

void ControllerInterface::onSendClicked(){
    emit(ControllerInterface::send());
}

void ControllerInterface::onResetClicked(){
    emit(ControllerInterface::reset());
}

void ControllerInterface::onConfigClicked(){
    emit(ControllerInterface::applyConfig());
}

void ControllerInterface::onImportConfigButtonClicked(){
    LOG_WARN("Create Import Dialog", NULL);
    emit(getCurrentPath(true));
}

void ControllerInterface::openImportDialog(QString current_path){
    LOG_WARN("Open Import Dialog", NULL);
    std::string path = current_path.toUtf8().constData();
    ied = new ImportExportDialog(this, true, path);
    connect(ied, SIGNAL(importConfig(QString, QString)), this, SLOT(onImportConfig(QString, QString)));
}

void ControllerInterface::onImportConfig(QString path, QString section_name){
    emit(importConfig(path, section_name));
    LOG_WARN("Delete Import dialog", NULL);
    delete ied;
}

void ControllerInterface::onExportConfigButtonClicked(){
    LOG_WARN("Create Export Dialog", NULL);
    emit(getCurrentPath(false));
}

void ControllerInterface::openExportDialog(QString current_path){
    LOG_WARN("Open Export Dialog", NULL);
    std::string path = current_path.toUtf8().constData();
    ied = new ImportExportDialog(this, false, path);
    connect(ied, SIGNAL(exportConfig(QString, QString)), this, SLOT(onExportConfig(QString, QString)));
}

void ControllerInterface::onExportConfig(QString path, QString section_name){
    emit(exportConfig(path, section_name));
    LOG_WARN("Delete Export dialog", NULL);
    delete ied;
}
