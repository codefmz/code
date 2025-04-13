file(GLOB SRC "./*.cpp")

add_library(log STATIC ${SRC})
set_target_properties(log PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) #依赖了log库的目标，也会依赖log库的头文件