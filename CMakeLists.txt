cmake_minimum_required(VERSION 3.1.0)

project(
    Plume
    VERSION 0.1
    DESCRIPTION "An application designed to replace INKK for INSA Lyon"
    LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(SOURCES
    "src/main.cpp"
    "src/plume.cpp"
    "src/widgets/bar/bottombar.cpp"
    "src/widgets/bar/button.cpp"
    "src/widgets/home/home.cpp"
    "src/widgets/calendar/calendar.cpp"
    "src/widgets/mail/mail.cpp"
)

set(HEADERS
    "src/plume.h"
    "src/widgets/bar/bottombar.h"
    "src/widgets/bar/button.h"
    "src/widgets/home/home.h"
    "src/widgets/calendar/calendar.h"
    "src/widgets/mail/mail.h"
)

qt_add_resources(SOURCES "src/plume.qrc")

add_executable(Plume ${SOURCES} ${HEADERS})

target_link_libraries(Plume Qt6::Widgets)
