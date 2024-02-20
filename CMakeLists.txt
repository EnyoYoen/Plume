cmake_minimum_required(VERSION 3.1.0)

project(
    Plume
    VERSION 0.1
    DESCRIPTION "An application designed to replace INKK for INSA Lyon"
    LANGUAGES C CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

set(SOURCES
    "src/main.cpp"
    "src/plume.cpp"
    "src/widgets/bar/bottombar.cpp"
    "src/widgets/bar/button.cpp"
    "src/widgets/home/home.cpp"
    "src/widgets/calendar/calendar.cpp"
    "src/widgets/calendar/event.cpp"
    "src/widgets/calendar/eventpage.cpp"
    "src/widgets/calendar/button.cpp"
    "src/widgets/mail/mail.cpp"
)

set(HEADERS
    "src/plume.h"
    "src/widgets/bar/bottombar.h"
    "src/widgets/bar/button.h"
    "src/widgets/home/home.h"
    "src/widgets/calendar/calendar.h"
    "src/widgets/calendar/event.h"
    "src/widgets/calendar/eventpage.h"
    "src/widgets/calendar/button.h"
    "src/widgets/mail/mail.h"
)

include_directories(
  libs/libical/src
  libs/libical/src/libical
  libs/libical/src/libicalss
  libs/libical/build/src
  libs/libical/build/src/libical
  libs/libical/build/src/libicalss
)

link_directories(
    libs/libical/build/lib
    libs/libical/lib
)

qt_add_resources(SOURCES "src/plume.qrc")

add_executable(Plume ${SOURCES} ${HEADERS})

target_link_libraries(Plume Qt6::Widgets)
target_link_libraries(Plume Qt6::Network)

target_link_libraries(Plume
    ical
    icalss
    icalvcal
)
