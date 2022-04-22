# builds & links builder module
ADD_LIBRARY(BUILDER STATIC 
    src/builder/builder.c
    src/builder/assembler.c)
TARGET_LINK_LIBRARIES(${EXE} BUILDER)