cmake_minimum_required(VERSION 3.16)

project(3DRec LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти Qt 6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Найти VTK
# Укажите путь к VTK
set(VTK_DIR "/usr/local/lib/cmake/vtk-9.4")

# Найдите и подключите модули VTK
find_package(VTK REQUIRED)

# Подключите VTK к проекту
include(${VTK_USE_FILE})

message(STATUS "VTK version: ${VTK_VERSION}")

# Автоматическое добавление moc, rcc и uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Добавить исполняемый файл
add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

include_directories(${VTK_DIR})

# Подключить пути и библиотеки
target_include_directories(${PROJECT_NAME} PRIVATE ${VTK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets ${VTK_LIBRARIES})

# Инициализация VTK
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)

