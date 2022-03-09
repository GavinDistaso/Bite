# builds & links parser module
ADD_LIBRARY(PARSER STATIC src/parser/parser.c)
TARGET_LINK_LIBRARIES(${EXE} PARSER)