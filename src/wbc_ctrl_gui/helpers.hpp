#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>

class QDoubleSpinBox;
class QLabel;
class QString;

QDoubleSpinBox* createDefaultSpinBox();
QLabel* createDefaultLabel(const QString &name, const bool bold);

#endif // HELPERS_HPP
