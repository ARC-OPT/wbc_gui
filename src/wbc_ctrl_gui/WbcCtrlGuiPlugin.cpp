#include "WbcCtrlGuiPlugin.hpp"
#include "WbcCtrlGui.hpp"

Q_EXPORT_PLUGIN2(WbcCtrlGui, WbcCtrlGuiPlugin)

WbcCtrlGuiPlugin::WbcCtrlGuiPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

WbcCtrlGuiPlugin::~WbcCtrlGuiPlugin()
{
}

bool WbcCtrlGuiPlugin::isContainer() const
{
    return false;
}

bool WbcCtrlGuiPlugin::isInitialized() const
{
    return initialized;
}

QIcon WbcCtrlGuiPlugin::icon() const
{
    return QIcon("");
}

QString WbcCtrlGuiPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"WbcCtrlGui\" name=\"wbcctrlgui\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>300</width>\n"
            "     <height>120</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>WbcCtrlGui</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>WbcCtrlGui</string>\n"
//            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString WbcCtrlGuiPlugin::group() const {
    return "Rock-Robotics";
}

QString WbcCtrlGuiPlugin::includeFile() const {
    return "WbcCtrlGui/WbcCtrlGui.hpp";
}

QString WbcCtrlGuiPlugin::name() const {
    return "WbcCtrlGui";
}

QString WbcCtrlGuiPlugin::toolTip() const {
    return whatsThis();
}

QString WbcCtrlGuiPlugin::whatsThis() const
{
    return "";
}

QWidget* WbcCtrlGuiPlugin::createWidget(QWidget *parent)
{
    return new WbcCtrlGui(parent);
}

void WbcCtrlGuiPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
