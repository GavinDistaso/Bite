# builds & links builder module
ADD_LIBRARY(BUILDER STATIC src/builder/bld-parser.c)
TARGET_LINK_LIBRARIES(${EXE} BUILDER)