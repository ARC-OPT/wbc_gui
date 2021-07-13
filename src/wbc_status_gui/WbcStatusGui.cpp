#include "WbcStatusGui.hpp"
#include <ui_ConstraintUI.h>
#include <ui_WbcStatusGui.h>
#include <ui_JointUI.h>
#include "ConstraintUI.hpp"
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
    clearConstraintUi();
    clearSolverUi();
    delete ui;

    if (import_dialog)
        delete import_dialog;

    if (export_dialog)
        delete export_dialog;
}

void WbcStatusGui::updateConstraint(const wbc::ConstraintStatus &constraint){
    ((ConstraintUI*)getConstraintInterface(QString::fromStdString(constraint.config.name)))->updateConstraint(constraint);
}

void WbcStatusGui::clearConstraintUi(){
    QGridLayout* constraints_layout = (QGridLayout*)ui->groupBoxWBCTasks->layout();

    ConstraintInterfaceMap::iterator it;
    for(it = constraint_uis.begin(); it != constraint_uis.end(); it++)
    {
        disconnect(it->second);
        disconnect(it->second);
        disconnect(it->second);

        constraints_layout->removeWidget(it->second);
        delete it->second;
    }
    constraint_uis.clear();
    for(uint i = 0; i < priority_labels.size(); i++){
        constraints_layout->removeWidget(priority_labels[i]);
        delete priority_labels[i];
    }
    priority_labels.clear();
    if(constraints_layout)
        delete constraints_layout;
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

void WbcStatusGui::configureConstraintUI(const std::vector<wbc::ConstraintConfig> &config){
    clearConstraintUi();

    ui->groupBoxWBCTasks->setLayout(new QGridLayout());
    QGridLayout* constraints_layout = (QGridLayout*)ui->groupBoxWBCTasks->layout();
    constraints_layout->setContentsMargins(10,-10,0,0);

    std::vector< std::vector<wbc::ConstraintConfig> > sorted_config;
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

            //Add constraint uis to grid layout. Row: Priority. Column: Ascending order
            ConstraintUI* constraint_ui = new ConstraintUI(this, QString::fromStdString(sorted_config[i][j].name));
            QPalette color;
            color.setColor(QPalette::Window, Qt::white);
            constraint_ui->setStyleSheet("");
            constraint_ui->setPalette(color);
            constraint_ui->setAutoFillBackground(true);
            constraint_ui->setPriority(sorted_config[i][0].priority);

            constraints_layout->addWidget(constraint_ui, row, column++);
            constraint_uis[QString::fromStdString(sorted_config[i][j].name)] = constraint_ui;

        }
        row++;
    }

    ui->groupBoxWBCTasks->setLayout(constraints_layout);

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
    QGridLayout* constraints_layout = (QGridLayout*)ui->groupBoxWBCTasks->layout();
    constraints_layout->addWidget(label, row, 0);
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
    if((int)ctrl_out.size() != solver_ui->getWeights().size())
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

void WbcStatusGui::sortConfigByPriority(const std::vector<wbc::ConstraintConfig>& config, std::vector< std::vector<wbc::ConstraintConfig> >& sorted_config){

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

QWidget* WbcStatusGui::getConstraintInterface(const QString &name){
    if(constraint_uis.count(name) == 0)
        throw std::runtime_error("No such constraint: " + name.toStdString());
    return constraint_uis[name];
}
