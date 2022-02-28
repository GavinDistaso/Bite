# builds & links builder module
ADD_LIBRARY(CLI STATIC 
    src/cli/cli-logger.c
    src/cli/cli-parser.c)
TARGET_LINK_LIBRARIES(${EXE} CLI)