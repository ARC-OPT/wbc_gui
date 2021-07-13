#include "ConstraintUI.hpp"
#include "ui_ConstraintUI.h"
#include <sstream>

ConstraintUI::ConstraintUI(QWidget *parent, QString title) :
    QWidget(parent),
    ui(new Ui::ConstraintWidget){
    ui->setupUi(this);
    ui->labelActive->setAutoFillBackground(true);
    connect(ui->bActivate, SIGNAL(clicked()), this, SLOT(onActivateClicked()));
    connect(ui->bExpand, SIGNAL(clicked()), this, SLOT(onExpandClicked()));
    connect(ui->bnChangePriority, SIGNAL(clicked()), this, SLOT(onPriorityChanged()));
    initialized_ = false;
    ui->scrollArea->hide();
    ui->labelTaskName->setText(title);
    cur_constraint.activation = -1;
    cur_constraint.timeout = -1;
}

ConstraintUI::~ConstraintUI(){
    disconnect(ui->bActivate);
    disconnect(ui->bExpand);
    for(uint i = 0; i < labels.size(); i++){
        ui->gridLayout->removeWidget(labels[i]);
        delete labels[i];
    }
    for(uint i = 0; i < line_edits.size(); i++){
        ui->gridLayout->removeWidget(line_edits[i]);
        delete line_edits[i];
    }
    for(uint i = 0; i < cbs.size(); i++){
        ui->gridLayout->removeWidget(cbs[i]);
        delete cbs[i];
    }

    delete ui;
}

void ConstraintUI::updateConstraint(const wbc::ConstraintStatus &constraint){

    if(constraint.activation != cur_constraint.activation || constraint.timeout != cur_constraint.timeout){
        std::stringstream ss;
        ss << "Active: " << constraint.activation << " / \nTimeout: " << constraint.timeout;
        ui->labelActive->setText(QString::fromStdString(ss.str()));

        QString style_sheet("");
        if( constraint.activation &&  !constraint.timeout ){ // Green label
            style_sheet = "QLabel { background:  rgba(0, 255, 0, 255); border-radius: 5px;}";
            ui->bActivate->setText("Off");
        }
        else if( (constraint.activation && constraint.timeout) ){ // Orange label
            style_sheet = "QLabel { background:  rgba(255, 100, 0, 255); border-radius: 5px;}";
            ui->bActivate->setText("Off");
        }
        else{ // red label
            style_sheet = "QLabel { background:  rgba(255, 0, 0, 255); border-radius: 5px;}";
            ui->bActivate->setText("On");
        }
        ui->labelActive->setStyleSheet(style_sheet);
    }

    if(!initialized_){
        std::vector<std::string> var_names;
        if(constraint.config.type == wbc::jnt){
            var_names = constraint.config.joint_names;
        }
        else{
            var_names.resize(6);
            var_names.assign(CART_VAR_NAMES, CART_VAR_NAMES+6);
        }

        for(uint i = 0; i < constraint.config.nVariables(); i++)
        {
            QLabel* label = makeFixedSizeLabel("label_name_", i, 70, 1000, true);
            ui->gridLayout->addWidget(label, 0, i+1);
            getLabel("label_name_", i)->setText(QString::fromStdString(var_names[i]));
            labels.push_back(label);

            label = makeFixedSizeLabel("label_y_ref_", i, 70, 60);
            ui->gridLayout->addWidget(label, 1, i+1);
            labels.push_back(label);

            label = makeFixedSizeLabel("label_y_solution_", i, 70, 60);
            ui->gridLayout->addWidget(label, 2, i+1);
            labels.push_back(label);

            label = makeFixedSizeLabel("label_y_", i, 70, 60);
            ui->gridLayout->addWidget(label, 3, i+1);

            QLineEdit *line_edit = makeLineEdit("line_edit_weights_", i, 70, 60);
            ui->gridLayout->addWidget(line_edit, 4, i+1);
            line_edits.push_back(line_edit);
        }

        QCheckBox* cb = new QCheckBox(this);
        cb->setChecked(true);
        cb->setText("update");
        QFont font;
        font.setPointSize(7);
        cb->setFont(font);
        cb->setObjectName("cbUpdateWeights");
        cbs.push_back(cb);
        ui->gridLayout->addWidget(cb, 5, constraint.config.nVariables() + 2);
        show();
        initialized_ = true;
    }

    if(ui->scrollArea->isVisible()){
        for(uint i = 0; i < constraint.config.nVariables(); i++)
        {
            getLabel("label_y_ref_", i)->setText(QString::number(constraint.y_ref(i), 'g', 4));
            getLabel("label_y_solution_", i)->setText(QString::number(constraint.y_solution(i), 'g', 4));
            getLabel("label_y_", i)->setText(QString::number(constraint.y(i), 'g', 4));
            handleWeights(constraint.weights);
        }
    }

    cur_constraint = constraint;
}

void ConstraintUI::handleWeights(const base::VectorXd& weights){
    QCheckBox* cb = ui->scrollArea->findChild<QCheckBox *>("cbUpdateWeights");
    if(!cb)
        throw std::invalid_argument("Cannot find check box with id cbUpdateWeights");

    for(uint i = 0; i < weights.size(); i++){
        QLineEdit* edit = getLineEdit("line_edit_weights_", i);
        if(cb->isChecked()){
            edit->setDisabled(true);
            edit->setText(QString::number(weights(i), 'g', 4));
        }
        else
            edit->setDisabled(false);
    }
}

QLineEdit* ConstraintUI::makeLineEdit(const std::string &prefix, const uint id, const int min_width, const int max_width){
    QLineEdit* line_edit = new QLineEdit(this);
    std::stringstream ss;
    ss << prefix << id;
    line_edit->setObjectName(QString::fromStdString(ss.str()));
    line_edit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    line_edit->setMinimumWidth(min_width);
    line_edit->setMaximumWidth(max_width);
    line_edit->setMinimumHeight(20);
    line_edit->setMaximumHeight(20);
    line_edit->setAlignment(Qt::AlignCenter);
    connect(line_edit, SIGNAL(editingFinished()), this, SLOT(onTaskWeightsChanged()));
    return line_edit;
}

QLabel* ConstraintUI::makeFixedSizeLabel(const std::string &prefix, const uint id, const int min_width, const int max_width, bool bold){
    QLabel* label = new QLabel(this);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label->setMinimumWidth(min_width);
    label->setMaximumWidth(max_width);
    QFont font;
    font.setBold(bold);
    font.setPointSize(7);
    label->setFont(font);
    std::stringstream ss;
    ss << prefix << id;
    label->setObjectName(QString::fromStdString(ss.str()));
    return label;
}

QLabel* ConstraintUI::getLabel(const std::string &prefix, const uint id){

    std::stringstream ss;
    ss << prefix << id;
    QLabel* label = ui->scrollArea->findChild<QLabel *>(ss.str().c_str());
    if(!label)
        throw std::invalid_argument("Cannot find label with id " + ss.str());
    return label;
}

QLineEdit* ConstraintUI::getLineEdit(const std::string &prefix, const uint id){
    std::stringstream ss;
    ss << prefix << id;
    QLineEdit* line_edit = ui->scrollArea->findChild<QLineEdit *>(ss.str().c_str());
    if(!line_edit)
        throw std::invalid_argument("Cannot find line edit with id " + ss.str());
    return line_edit;
}

QVector<double> ConstraintUI::getTaskWeights(){
    uint no_vars;
    if(cur_constraint.config.type == wbc::jnt)
        no_vars = cur_constraint.config.joint_names.size();
    else
        no_vars = 6;
    QVector<double> weights(no_vars);
    for (uint i = 0; i < no_vars; i++) {
        weights[i] = getLineEdit("line_edit_weights_", i)->text().toDouble();
    }

    return weights;
}

double ConstraintUI::getActivation(){
    return cur_constraint.activation;
}

int ConstraintUI::getPriority() {
    return cur_constraint.config.priority;
}

void ConstraintUI::onActivateClicked(){
    if(cur_constraint.activation > 0)
        emit(ConstraintUI::deactivate());
    else
        emit(ConstraintUI::activate());
}

void ConstraintUI::onPriorityChanged(){
    bool ok = false;
    int new_prio = QInputDialog::getInt(this, "Set new priority",
                                            "Priority:", getPriority(), 0, 1000, 1 , &ok);
    if (ok and new_prio >= 0){
        setPriority(new_prio);
        emit(ConstraintUI::priorityChanged(new_prio));
    }
}

void ConstraintUI::onTaskWeightsChanged(){
    emit(ConstraintUI::taskWeightsChanged());
}

void ConstraintUI::onExpandClicked(){
    if(ui->scrollArea->isHidden())
        ui->scrollArea->show();
    else
        ui->scrollArea->hide();
    ui->scrollArea->adjustSize();
    ui->groupBox->adjustSize();
    this->adjustSize();
}

void ConstraintUI::setPriority(int prio){
    cur_constraint.config.priority = prio;
}

bool ConstraintUI::expanded(){
    return !ui->scrollArea->isHidden();
}

