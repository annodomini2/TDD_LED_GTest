add_library(util INTERFACE)

target_sources(util INTERFACE 
    FILE_SET HEADERS 
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES "RuntimeError.h")

target_include_directories(util INTERFACE .)