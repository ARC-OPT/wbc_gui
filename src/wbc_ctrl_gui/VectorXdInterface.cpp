#include "VectorXdInterface.hpp"
#include "helpers.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>

////// InputPortInterface
///
InputPortInterface::InputPortInterface(QWidget* parent, int size, QGridLayout *parent_layout) :
    VectorXdInterface(parent),
    layout(parent_layout){

    int row = layout->rowCount();

    for(int i = 0; i < size; i++)
        addSpinBox(row, i);
}

void InputPortInterface::addSpinBox(int row, int column){
    QDoubleSpinBox* spin_box = createDefaultSpinBox();
    layout->addWidget(spin_box, row, column);
    layout->setAlignment(spin_box, Qt::AlignHCenter | Qt::AlignTop);
    spin_boxes.push_back(spin_box);
}

void InputPortInterface::update(const QVector<double> &values){
    for(size_t i = 0; i < spin_boxes.size(); i++)
        spin_boxes[i]->setValue(values[i]);
}

QVector<double> InputPortInterface::query(){
    QVector<double> values(size());
    for(int i = 0; i < size(); i++)
        values[i] = spin_boxes[i]->value();
    return values;
}

////// OutputPortInterface
///
OutputPortInterface::OutputPortInterface(QWidget* parent, int size, QGridLayout *parent_layout) :
    VectorXdInterface(parent),
    layout(parent_layout){

    int row = layout->rowCount();
    for(int i = 0; i < size; i++)
        addLabel(row, i);
}

void OutputPortInterface::addLabel(int row, int column){
    QLabel* label = createDefaultLabel(".nan", false);
    layout->addWidget(label, row, column);
    layout->setAlignment(label, Qt::AlignHCenter | Qt::AlignTop);
    labels.push_back(label);
}

void OutputPortInterface::update(const QVector<double> &values){
    for(int i = 0; i < size(); i++)
        labels[i]->setText(QString::number(values[i], 'g', 4));
}

QVector<double> OutputPortInterface::query(){
    QVector<double> values(size());
    for(int i = 0; i < size(); i++)
        values[i] = labels[i]->text().toDouble();
    return values;
}

////// ConfigInterface
///
ConfigInterface::ConfigInterface(QWidget* parent, int size, QGridLayout* parent_layout) :
    VectorXdInterface(parent),
    layout(parent_layout){

    int row = layout->rowCount();
    for(int i = 0; i < size; i++)
        addSpinBox(row, i);
    addSpinBox(row, size); // "All" spin box

    // Connect "All" spin box --> All other spin boxes should be set to this value, if it has been changed by the user
    connect(spin_boxes[size], SIGNAL(valueChanged(double)), this, SLOT(onAllSpinBoxChanged(double)));
}

void ConfigInterface::addSpinBox(int row, int column){
    QDoubleSpinBox* spin_box = createDefaultSpinBox();
    layout->addWidget(spin_box, row, column);
    layout->setAlignment(spin_box, Qt::AlignHCenter | Qt::AlignTop);
    spin_boxes.push_back(spin_box);
}

void ConfigInterface::update(const QVector<double> &values){
    for(int i = 0; i < size(); i++)
        spin_boxes[i]->setValue(values[i]);
}

QVector<double> ConfigInterface::query(){
    QVector<double> values(size());
    for(int i = 0; i < size(); i++)
        values[i] = spin_boxes[i]->value();
    return values;
}

void ConfigInterface::onAllSpinBoxChanged(double value){
    QVector<double> newValue(size(), value);
    update(newValue);
}
