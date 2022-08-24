#ifndef WBCSTATUSGUI_HPP
#define WBCSTATUSGUI_HPP

#include <QtGui>
#include <base/commands/Joints.hpp>
#include <wbc/core/TaskStatus.hpp>
#include "ImportExportDialog.hpp"
#include <wbc/core/QuadraticProgram.hpp>

class TaskUI;
class JointUI;

namespace Ui{
    class MainWidget;
}

typedef std::map<QString, TaskUI*> TaskInterfaceMap;

class WbcStatusGui : public QWidget
{
    Q_OBJECT
public:
    WbcStatusGui(QWidget *parent = 0);
    virtual ~WbcStatusGui();

public slots:
    QWidget* getTaskInterface(const QString &name);
    void updateTask(const wbc::TaskStatus &task);
    void configureTaskUI(const std::vector<wbc::TaskConfig> &config);
    void configureSolverUI(const std::vector<std::string>& joint_names);
    void updateCtrlOut(const base::commands::Joints& ctrl_out);
    void updateJointWeights(const wbc::JointWeights& weight);
    wbc::JointWeights getJointWeights();

protected slots:
    void onJointWeightsChanged();
    void onImportConfigButtonClicked();
    void onImportConfig(QString path, QString section_name);
    void onExportConfigButtonClicked();
    void onExportConfig(QString path, QString section_name);
    void onReconfigureWBCClicked();

signals:
    void jointWeightsChanged();
    void importConfig(QString path, QString section_name);
    void exportConfig(QString path, QString section_name);
    void reconfigureWBC();

protected:
    void addPriorityLabel(uint prio, uint row);
    void sortConfigByPriority(const std::vector<wbc::TaskConfig>& config,
                              std::vector< std::vector<wbc::TaskConfig> >& sorted_config);
    void clearTaskUi();
    void clearSolverUi();

private:
    TaskInterfaceMap task_uis;
    std::vector<QLabel*> priority_labels;
    Ui::MainWidget *ui;
    JointUI* solver_ui;
    ImportExportDialog *import_dialog, *export_dialog;
};

#endif /* WBCSTATUSGuiHPP */
