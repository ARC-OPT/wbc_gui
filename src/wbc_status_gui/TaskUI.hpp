#ifndef TASKUI_HPP
#define TASKUI_HPP

#include <QWidget>
#include <wbc/core/TaskStatus.hpp>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QInputDialog>

namespace Ui {
class TaskWidget;
}

const std::string CART_VAR_NAMES[6] = {"X", "Y", "Z", "rotX", "rotY", "rotZ"};

class TaskUI : public QWidget
{
    Q_OBJECT

public:
    explicit TaskUI(QWidget* parent, QString titel = "Wbc Task");
    ~TaskUI();

    void setPriority(int priority);
    bool expanded();

public slots:
    void updateTask(const wbc::TaskStatus& task);
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

    Ui::TaskWidget* ui;
    std::vector<QLabel*> labels;
    std::vector<QLineEdit*> line_edits;
    std::vector<QCheckBox*> cbs;
    wbc::TaskStatus cur_task;

    QLabel* getLabel(const std::string &prefix, const uint id);
    QLineEdit* getLineEdit(const std::string &prefix, const uint id);
    QLabel* makeFixedSizeLabel(const std::string &prefix, const uint id, const int min_width, const int max_width, bool bold = false);
    QLineEdit* makeLineEdit(const std::string &prefix, const uint id, const int min_width, const int max_width);
    void handleWeights(const base::VectorXd& weights);
};

#endif
