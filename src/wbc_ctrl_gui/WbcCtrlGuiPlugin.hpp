#ifndef WbcCtrlGuiPlugin_HPP
#define WbcCtrlGuiPlugin_HPP

#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>

class WbcCtrlGuiPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")

public:
    WbcCtrlGuiPlugin(QObject *parent = 0);
    virtual ~WbcCtrlGuiPlugin();

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget* createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool initialized;
};

#endif /* WbcCtrlGuiPlugin_HPP */
