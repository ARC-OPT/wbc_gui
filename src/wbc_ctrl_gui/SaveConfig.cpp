#include "SaveConfig.hpp"

SaveConfig::SaveConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveConfigWidget){
    ui->setupUi(this);
    connect(ui->bSave, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(ui->bSelectAll, SIGNAL(clicked()), this, SLOT(onSelectAllClicked()));
    connect(ui->bSelectFile, SIGNAL(clicked()), this, SLOT(onSelectFileClicked()));
    select_all = true;
}

SaveConfig::~SaveConfig(){
    delete ui;
    controllers.clear();
}

void SaveConfig::add_controller(std::string name, ControllerInterface* controller)
{
    controllers[name] = controller;
}

void SaveConfig::onSaveClicked(){

    QString filename =  ui->lineEditConfig->text();
    if(filename == "<auto>")
    {
        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();
        filename =  date.toString("yyyyMMdd") + "_" + time.toString("hhmmss") + ".yml";
    }
    //for(controllerInterfaceMap::iterator it = controllers.begin(); it != controllers.end(); ++it)
    //    if(it->second->isSaveChecked()) emit(SaveConfig::saveSignal(QString::fromStdString(it->first),filename));
}


void SaveConfig::onSelectAllClicked(){

    //for(controllerInterfaceMap::iterator it = controllers.begin(); it != controllers.end(); ++it)
    //    it->second->setSave(select_all);

    if(select_all)
        ui->bSelectAll->setText("Unselect all");
    else
        ui->bSelectAll->setText("Select all");

    select_all = !select_all;
}

void SaveConfig::onSelectFileClicked() {
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open .yaml file"), "/home", tr("YAML Files (*.yaml)"));
    ui->lineEditConfig->setText(file_name);
}
