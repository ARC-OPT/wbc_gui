#ifndef VECTORXDINTERFACE_HPP
#define VECTORXDINTERFACE_HPP

#include <QWidget>

class QGridLayout;
class QLabel;
class QDoubleSpinBox;

////// Base Class
///
class VectorXdInterface : public QWidget
{
    Q_OBJECT
public:
    VectorXdInterface(QWidget* parent) :
        QWidget(parent){}
    ~VectorXdInterface(){}

    virtual void update(const QVector<double>& values) = 0;
    virtual QVector<double> query() = 0;
    virtual int size() = 0;
};

////// InputPortInterface
///
class InputPortInterface : public VectorXdInterface{
    Q_OBJECT
protected:
    QGridLayout* layout;
    std::vector<QDoubleSpinBox*> spin_boxes;

    void addSpinBox(int row, int column);

public:
    InputPortInterface(QWidget* parent, int size, QGridLayout* parent_layout);

    virtual void update(const QVector<double>& values);
    virtual QVector<double> query();
    virtual int size(){return spin_boxes.size();}

};

////// OutputPortInterface
///
class OutputPortInterface : public VectorXdInterface{
    Q_OBJECT
protected:
    QGridLayout* layout;
    std::vector<QLabel*> labels;

    void addLabel(int row, int column);

public:
    OutputPortInterface(QWidget* parent, int size, QGridLayout* parent_layout);

    virtual void update(const QVector<double>& values);
    virtual QVector<double> query();
    virtual int size(){return labels.size();}
};

////// ConfigInterface
///
class ConfigInterface : public VectorXdInterface{
    Q_OBJECT
protected:
    QGridLayout* layout;
    std::vector<QDoubleSpinBox*> spin_boxes;

    void addSpinBox(int row, int column);

public slots:
    void onAllSpinBoxChanged(double value);

public:
    ConfigInterface(QWidget* parent, int size, QGridLayout* parent_layout);
    ~ConfigInterface(){}

    virtual void update(const QVector<double>& values);
    virtual QVector<double> query();
    virtual int size(){return spin_boxes.size()-1;}
};

#endif // VECTORXDINTERFACE_HPP
