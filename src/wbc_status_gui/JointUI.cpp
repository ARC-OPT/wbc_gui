#include "JointUI.hpp"
#include "ui_JointUI.h"

JointUI::JointUI(QWidget *parent, const std::vector<std::string> &joint_names) :
    QWidget(parent),
    ui(new Ui::JointWidget),
    joint_names(joint_names){

    ui->setupUi(this);
    for(uint i = 0; i < joint_names.size(); i++)
    {
        QLabel* label = getLabel(QString::fromStdString(joint_names[i]));
        label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        ui->gridLayout->addWidget(label, i+1,0);
        labels_joint_names_.push_back(label);

        label = getLabel(QString::number(0.0));
        labels_pos_.push_back(label);
        ui->gridLayout->addWidget(label, i+1, 1);

        label = getLabel(QString::number(0.0));
        labels_vel_.push_back(label);
        ui->gridLayout->addWidget(label, i+1, 2);

        label = getLabel(QString::number(0.0));
        labels_acc_.push_back(label);
        ui->gridLayout->addWidget(label, i+1, 3);

        label = getLabel(QString::number(0.0));
        labels_tau_.push_back(label);
        ui->gridLayout->addWidget(label, i+1, 4);

        QLineEdit* line_edit = new QLineEdit(this);
        ui->gridLayout->addWidget(line_edit, i+1,5);
        line_edit_joint_weights_.push_back(line_edit);
        line_edit->setAlignment(Qt::AlignCenter);
        line_edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        line_edit->setMinimumSize(60,15);
        line_edit->setMaximumSize(60,15);
        connect(line_edit, SIGNAL(editingFinished()), this, SLOT(onWeightChanged()));
    }

    cb = new QCheckBox("update", this);
    QFont font;
    font.setPointSize(8);
    cb->setFont(font);
    cb->setChecked(true);

    ui->gridLayout->addWidget(cb, joint_names.size()+2, 5);
}

JointUI::~JointUI(){
    for(uint i = 0; i < line_edit_joint_weights_.size(); i++)
    {
        disconnect(line_edit_joint_weights_[i]);
        ui->gridLayout->removeWidget(line_edit_joint_weights_[i]);
        delete line_edit_joint_weights_[i];
    }
    for(uint i = 0; i < labels_pos_.size(); i++)
    {
        ui->gridLayout->removeWidget(labels_pos_[i]);
        delete labels_pos_[i];
    }
    for(uint i = 0; i < labels_vel_.size(); i++)
    {
        ui->gridLayout->removeWidget(labels_vel_[i]);
        delete labels_vel_[i];
    }
    for(uint i = 0; i < labels_acc_.size(); i++)
    {
        ui->gridLayout->removeWidget(labels_acc_[i]);
        delete labels_acc_[i];
    }
    for(uint i = 0; i < labels_tau_.size(); i++)
    {
        ui->gridLayout->removeWidget(labels_tau_[i]);
        delete labels_tau_[i];
    }
    for(uint i = 0; i < labels_joint_names_.size(); i++)
    {
        ui->gridLayout->removeWidget(labels_joint_names_[i]);
        delete labels_joint_names_[i];
    }

    delete ui;
}

QLabel* JointUI::getLabel(const QString name){
    QLabel* label = new QLabel(name, this);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label->setMinimumSize(80,20);
    label->setMaximumSize(80,20);
    QFont font;
    font.setPointSize(7);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    return label;
}

void JointUI::updateCtrlOut(const base::commands::Joints& ctrl_out){

    for(uint i = 0; i < ctrl_out.size(); i++){
        labels_pos_[i]->setText(QString::number(ctrl_out[i].position, 'g', 4));
        labels_vel_[i]->setText(QString::number(ctrl_out[i].speed, 'g', 4));
        labels_acc_[i]->setText(QString::number(ctrl_out[i].acceleration, 'g', 4));
        labels_tau_[i]->setText(QString::number(ctrl_out[i].effort, 'g', 4));
    }
}

void JointUI::updateWeights(const wbc::JointWeights& weights){
    for(uint i = 0; i < weights.size(); i++){
        if(cb->isChecked()){
            line_edit_joint_weights_[i]->setText(QString::number(weights[i], 'g', 4));
            line_edit_joint_weights_[i]->setDisabled(true);
        }
        else
            line_edit_joint_weights_[i]->setDisabled(false);
    }
}

wbc::JointWeights JointUI::getWeights(){
    wbc::JointWeights weights;
    weights.resize(joint_names.size());
    weights.names = joint_names;
    for(uint i = 0; i < joint_names.size(); i++)
        weights[joint_names[i]] = line_edit_joint_weights_[i]->text().toDouble();
    return weights;
}

void JointUI::onWeightChanged(){
    emit(JointUI::weightChanged());
}

