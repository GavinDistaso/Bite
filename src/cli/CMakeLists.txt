# builds & links builder module
ADD_LIBRARY(CLI STATIC 
    src/cli/logger.c
    src/cli/parser.c)
TARGET_LINK_LIBRARIES(${EXE} CLI)