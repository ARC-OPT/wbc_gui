#include "WbcStatusGui.hpp"
#include <ui_TaskUI.h>
#include <ui_WbcStatusGui.h>
#include <ui_JointUI.h>
#include "TaskUI.hpp"
#include "JointUI.hpp"

WbcStatusGui::WbcStatusGui(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWidget()),
      solver_ui(0){
    ui->setupUi(this);
    connect(ui->bnImportConfig, SIGNAL(clicked()), this, SLOT(onImportConfigButtonClicked()));
    connect(ui->bnExportConfig, SIGNAL(clicked()), this, SLOT(onExportConfigButtonClicked()));
    connect(ui->bnReconfigureWBC, SIGNAL(clicked()), this, SLOT(onReconfigureWBCClicked()));

    import_dialog = 0;
    export_dialog = 0;
}

WbcStatusGui::~WbcStatusGui(){
    clearTaskUi();
    clearSolverUi();
    delete ui;

    if (import_dialog)
        delete import_dialog;

    if (export_dialog)
        delete export_dialog;
}

void WbcStatusGui::updateTask(const wbc::TaskStatus &task){
    ((TaskUI*)getTaskInterface(QString::fromStdString(task.config.name)))->updateTask(task);
}

void WbcStatusGui::clearTaskUi(){
    QGridLayout* tasks_layout = (QGridLayout*)ui->groupBoxWBCTasks->layout();

    TaskInterfaceMap::iterator it;
    for(it = task_uis.begin(); it != task_uis.end(); it++)
    {
        disconnect(it->second);
        disconnect(it->second);
        disconnect(it->second);

        tasks_layout->removeWidget(it->second);
        delete it->second;
    }
    task_uis.clear();
    for(uint i = 0; i < priority_labels.size(); i++){
        tasks_layout->removeWidget(priority_labels[i]);
        delete priority_labels[i];
    }
    priority_labels.clear();
    if(tasks_layout)
        delete tasks_layout;
}

void WbcStatusGui::clearSolverUi(){
    QVBoxLayout* solver_layout = (QVBoxLayout*)ui->groupBoxSolver->layout();
    if(solver_layout)
    {
        solver_layout->removeWidget(solver_ui);
        disconnect(solver_ui);
        delete solver_layout;
        delete solver_ui;
    }
    solver_layout = 0;
    solver_ui = 0;
}

void WbcStatusGui::configureTaskUI(const std::vector<wbc::TaskConfig> &config){
    clearTaskUi();

    ui->groupBoxWBCTasks->setLayout(new QGridLayout());
    QGridLayout* tasks_layout = (QGridLayout*)ui->groupBoxWBCTasks->layout();
    tasks_layout->setContentsMargins(10,-10,0,0);

    std::vector< std::vector<wbc::TaskConfig> > sorted_config;
    sortConfigByPriority(config, sorted_config);

    int row = 0, column = 1;
    for(size_t i = 0; i < sorted_config.size(); i++){

        addPriorityLabel(sorted_config[i][0].priority, row);

        column = 1;
        for(size_t j = 0; j < sorted_config[i].size(); j++){

            if(column > 3){
                column=1;
                row++;
            }

            //Add task uis to grid layout. Row: Priority. Column: Ascending order
            TaskUI* task_ui = new TaskUI(this, QString::fromStdString(sorted_config[i][j].name));
            QPalette color;
            color.setColor(QPalette::Window, Qt::white);
            task_ui->setStyleSheet("");
            task_ui->setPalette(color);
            task_ui->setAutoFillBackground(true);
            task_ui->setPriority(sorted_config[i][0].priority);

            tasks_layout->addWidget(task_ui, row, column++);
            task_uis[QString::fromStdString(sorted_config[i][j].name)] = task_ui;

        }
        row++;
    }

    ui->groupBoxWBCTasks->setLayout(tasks_layout);

    ui->groupBox->setContentsMargins(0,20,0,0);
    ui->groupBoxWBCTasks->setContentsMargins(0,20,10,10);
}

void WbcStatusGui::configureSolverUI(const std::vector<std::string> &joint_names){
    solver_ui = new JointUI(this, joint_names);
    connect(solver_ui, SIGNAL(weightChanged()), this, SLOT(onJointWeightsChanged()));
    ui->groupBoxSolver->setLayout(new QVBoxLayout());
    QVBoxLayout* solver_layout = (QVBoxLayout* )ui->groupBoxSolver->layout();
    solver_layout->addWidget(solver_ui);
}

void WbcStatusGui::addPriorityLabel(uint prio, uint row){
    QLabel* label = new QLabel(QString("Prio = ") + QString::number(prio));
    QFont font;
    font.setItalic(true);
    font.setBold(true);
    font.setPointSize(10);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumWidth(70);
    QGridLayout* tasks_layout = (QGridLayout*)ui->groupBoxWBCTasks->layout();
    tasks_layout->addWidget(label, row, 0);
    priority_labels.push_back(label);
}

wbc::JointWeights WbcStatusGui::getJointWeights(){
    return solver_ui->getWeights();
}

void WbcStatusGui::onJointWeightsChanged(){
    emit(jointWeightsChanged());
}

void WbcStatusGui::onReconfigureWBCClicked(){
    emit(reconfigureWBC());
}

void WbcStatusGui::updateCtrlOut(const base::commands::Joints& ctrl_out){
    if(ctrl_out.size() != solver_ui->getWeights().size())
        throw std::runtime_error("Size of joint weight vector should be " + std::to_string(solver_ui->getWeights().size()) +
                                 " but is " + std::to_string(ctrl_out.size()));
    solver_ui->updateCtrlOut(ctrl_out);
}

void WbcStatusGui::updateJointWeights(const wbc::JointWeights& weights){
    if(weights.size() != solver_ui->getWeights().size())
        throw std::runtime_error("Size of joint weight vector should be " + std::to_string(solver_ui->getWeights().size()) +
                                 " but is " + std::to_string(weights.size()));

    solver_ui->updateWeights(weights);
}

void WbcStatusGui::sortConfigByPriority(const std::vector<wbc::TaskConfig>& config, std::vector< std::vector<wbc::TaskConfig> >& sorted_config){

    // Get highest prio
    int max_prio = 0;
    for(uint i = 0; i < config.size(); i++){

        if(config[i].priority > max_prio)
            max_prio = config[i].priority;
    }
    sorted_config.resize(max_prio + 1);

    for(uint i = 0; i < config.size(); i++)
        sorted_config[config[i].priority].push_back(config[i]);

    // Erase empty priorities
    for(uint i = 0; i < sorted_config.size(); i++)
    {
        if(sorted_config[i].empty())
        {
            sorted_config.erase(sorted_config.begin() + i, sorted_config.begin() + i + 1);
            i--;
        }
    }
}

void WbcStatusGui::onImportConfigButtonClicked(){
    import_dialog = new ImportExportDialog(this, true, std::getenv("AUTOPROJ_CURRENT_ROOT"));
    connect(import_dialog, SIGNAL(importConfig(QString, QString)), this, SLOT(onImportConfig(QString, QString)));
}

void WbcStatusGui::onImportConfig(QString path, QString section_name){
    emit(importConfig(path, section_name));
    delete import_dialog;
}

void WbcStatusGui::onExportConfigButtonClicked(){
    export_dialog = new ImportExportDialog(this, false, std::getenv("AUTOPROJ_CURRENT_ROOT"));
    connect(export_dialog, SIGNAL(exportConfig(QString, QString)), this, SLOT(onExportConfig(QString, QString)));
}

void WbcStatusGui::onExportConfig(QString path, QString section_name){
    emit(exportConfig(path, section_name));
    delete export_dialog;
}

QWidget* WbcStatusGui::getTaskInterface(const QString &name){
    if(task_uis.count(name) == 0)
        throw std::runtime_error("No such task: " + name.toStdString());
    return task_uis[name];
}
