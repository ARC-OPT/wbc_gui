
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(MOC_HDRS
    WbcStatusGui.hpp
    WbcStatusGuiPlugin.hpp
    TaskUI.hpp
    JointUI.hpp
    ImportExportDialog.hpp
)

SET(HDRS
    WbcStatusGui.hpp
    TaskUI.hpp
    JointUI.hpp
    ImportExportDialog.hpp
)

FILE(GLOB SRCS
    *.cpp
)

FILE(GLOB UI_FILES
    *.ui
)

SET(QtApp_RCCS resources.qrc)

rock_vizkit_widget(wbc_status_gui
    SOURCES ${SRCS} ${QtApp_RCC_SRCS}
    HEADERS ${HDRS}
    MOC5 ${MOC_HDRS}
    DEPS_PKGCONFIG wbc-core
    DEPS_TARGET Qt5::Widgets
    UI ${UI_FILES}
)

QT5_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

