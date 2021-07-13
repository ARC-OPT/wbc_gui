#ifndef WBCSTATUSGUI_HPP
#define WBCSTATUSGUI_HPP

#include <QtGui>
#include <base/commands/Joints.hpp>
#include <wbc/core/ConstraintStatus.hpp>
#include "ImportExportDialog.hpp"
#include <wbc/core/QuadraticProgram.hpp>

class ConstraintUI;
class JointUI;

namespace Ui{
    class MainWidget;
}

typedef std::map<QString, ConstraintUI*> ConstraintInterfaceMap;

class WbcStatusGui : public QWidget
{
    Q_OBJECT
public:
    WbcStatusGui(QWidget *parent = 0);
    virtual ~WbcStatusGui();

public slots:
    QWidget* getConstraintInterface(const QString &name);
    void updateConstraint(const wbc::ConstraintStatus &constraint);
    void configureConstraintUI(const std::vector<wbc::ConstraintConfig> &config);
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
    void sortConfigByPriority(const std::vector<wbc::ConstraintConfig>& config,
                              std::vector< std::vector<wbc::ConstraintConfig> >& sorted_config);
    void clearConstraintUi();
    void clearSolverUi();

private:
    ConstraintInterfaceMap constraint_uis;
    std::vector<QLabel*> priority_labels;
    Ui::MainWidget *ui;
    JointUI* solver_ui;
    ImportExportDialog *import_dialog, *export_dialog;
};

#endif /* WBCSTATUSGuiHPP */
