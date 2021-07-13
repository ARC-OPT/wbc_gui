#include "WbcStatusGuiPlugin.hpp"
#include "WbcStatusGui.hpp"

Q_EXPORT_PLUGIN2(WbcStatusGui, WbcStatusGuiPlugin)

WbcStatusGuiPlugin::WbcStatusGuiPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

WbcStatusGuiPlugin::~WbcStatusGuiPlugin()
{
}

bool WbcStatusGuiPlugin::isContainer() const
{
    return false;
}

bool WbcStatusGuiPlugin::isInitialized() const
{
    return initialized;
}

QIcon WbcStatusGuiPlugin::icon() const
{
    return QIcon("");
}

QString WbcStatusGuiPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"WbcStatusGui\" name=\"wbcstatusgui\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>300</width>\n"
            "     <height>120</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>WbcStatusGui</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>WbcStatusGui</string>\n"
//            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString WbcStatusGuiPlugin::group() const {
    return "Rock-Robotics";
}

QString WbcStatusGuiPlugin::includeFile() const {
    return "WbcStatusGui/WbcStatusGui.hpp";
}

QString WbcStatusGuiPlugin::name() const {
    return "WbcStatusGui";
}

QString WbcStatusGuiPlugin::toolTip() const {
    return whatsThis();
}

QString WbcStatusGuiPlugin::whatsThis() const
{
    return "";
}

QWidget* WbcStatusGuiPlugin::createWidget(QWidget *parent)
{
    return new WbcStatusGui(parent);
}

void WbcStatusGuiPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
