#ifndef JOINTUI_HPP
#define JOINTUI_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <base/Eigen.hpp>
#include <base/commands/Joints.hpp>
#include <wbc/core/QuadraticProgram.hpp>

namespace Ui {
class JointWidget;
}

class JointUI : public QWidget
{
    Q_OBJECT

public:
    explicit JointUI(QWidget* parent, const std::vector<std::string> &joint_names);
    ~JointUI();

    void updateCtrlOut(const base::commands::Joints& ctrl_out);
    void updateWeights(const wbc::JointWeights& weights);
    wbc::JointWeights getWeights();
    QLabel* getLabel(const QString name);

    std::vector<std::string> joint_names;
    Ui::JointWidget* ui;
    std::vector<QLabel*> labels_pos_,labels_vel_,labels_acc_,labels_tau_;
    std::vector<QLineEdit*> line_edit_joint_weights_;
    std::vector<QLabel*> labels_joint_names_;
    QCheckBox* cb;
public slots:
    void onWeightChanged();

signals:
    void weightChanged();

};

#endif // JOINTUI_HPP
