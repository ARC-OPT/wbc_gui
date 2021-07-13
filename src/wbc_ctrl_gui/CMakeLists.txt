set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(MOC_HDRS
    WbcCtrlGui.hpp
    WbcCtrlGuiPlugin.hpp
    SaveConfig.hpp
    ImportExportDialog.hpp
    ControllerInterface.hpp
    VectorXdInterface.hpp
)

SET(HDRS
    WbcCtrlGui.hpp
    WbcCtrlGuiPlugin.hpp
    SaveConfig.hpp
    ImportExportDialog.hpp
    ControllerInterface.hpp
    VectorXdInterface.hpp
    helpers.hpp
)

FILE(GLOB SRCS
    *.cpp
)

FILE(GLOB UI_FILES
    *.ui
)

SET(QtApp_RCCS resources.qrc)

rock_vizkit_widget(wbc_ctrl_gui
    SOURCES ${SRCS} ${QtApp_RCC_SRCS}
    HEADERS ${HDRS}
    MOC ${MOC_HDRS}
    DEPS_PKGCONFIG QtCore QtGui base-lib
    UI ${UI_FILES}
)
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

