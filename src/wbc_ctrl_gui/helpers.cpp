#include "helpers.hpp"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QFont>
#include <QSizePolicy>

QDoubleSpinBox* createDefaultSpinBox(){

    QDoubleSpinBox* spinBox = new QDoubleSpinBox();
    QFont font;
    font.setPointSize(7);
    spinBox->setFont(font);
    spinBox->setDecimals(3);
    spinBox->setSingleStep(0.02);
    spinBox->setMaximum(1000);
    spinBox->setMinimum(-1000);
    spinBox->setMinimumWidth(56);
    spinBox->setMaximumWidth(56);
    spinBox->setMinimumHeight(20);
    spinBox->setMaximumHeight(20);
    spinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    return spinBox;
}

QLabel* createDefaultLabel(const QString &name, const bool bold){

    QLabel* label = new QLabel(name);
    QFont font;
    font.setBold(bold);
    font.setPointSize(7);
    label->setFont(font);
    label->adjustSize();
    label->setMinimumHeight(20);
    label->setMaximumHeight(20);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    return label;
}
