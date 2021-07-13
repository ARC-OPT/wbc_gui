#ifndef CONSTRAINTUI_HPP
#define CONSTRAINTUI_HPP

#include <QWidget>
#include <wbc/core/ConstraintStatus.hpp>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QInputDialog>

namespace Ui {
class ConstraintWidget;
}

const std::string CART_VAR_NAMES[6] = {"X", "Y", "Z", "rotX", "rotY", "rotZ"};

class ConstraintUI : public QWidget
{
    Q_OBJECT

public:
    explicit ConstraintUI(QWidget* parent, QString titel = "Wbc Task");
    ~ConstraintUI();

    void setPriority(int priority);
    bool expanded();

public slots:
    void updateConstraint(const wbc::ConstraintStatus& constraint);
    int getPriority();
    QVector<double> getTaskWeights();
    void onActivateClicked();
    void onTaskWeightsChanged();
    void onExpandClicked();
    void onPriorityChanged();
    double getActivation();

signals:
    void activate();
    void deactivate();
    void taskWeightsChanged();
    void priorityChanged(int i);
    void changePriority();

private:
    bool initialized_;

    Ui::ConstraintWidget* ui;
    std::vector<QLabel*> labels;
    std::vector<QLineEdit*> line_edits;
    std::vector<QCheckBox*> cbs;
    wbc::ConstraintStatus cur_constraint;

    QLabel* getLabel(const std::string &prefix, const uint id);
    QLineEdit* getLineEdit(const std::string &prefix, const uint id);
    QLabel* makeFixedSizeLabel(const std::string &prefix, const uint id, const int min_width, const int max_width, bool bold = false);
    QLineEdit* makeLineEdit(const std::string &prefix, const uint id, const int min_width, const int max_width);
    void handleWeights(const base::VectorXd& weights);
};

#endif
