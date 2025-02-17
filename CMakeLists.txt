# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_init(wbc_gui 0.1)
rock_feature(NOCURDIR)
rock_standard_layout()

INSTALL( PROGRAMS bin/wbc_ctrl_gui
    DESTINATION bin )

INSTALL( PROGRAMS bin/wbc_status_gui
    DESTINATION bin )

INSTALL( PROGRAMS bin/wbc_gui
    DESTINATION bin )

