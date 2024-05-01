########################
##
##  Created: Wed May 01 2024
##  File: Makefile
##
########################

SRC_PATH = src
BUILD_PATH = build
COMPILER = g++
FILE_TYPE = .cpp

SRC =\
	${SRC_PATH}/scene.cpp\
	${SRC_PATH}/world.cpp\

INCLUDE =\
	-I./include/\

OBJ = ${SRC:${SRC_PATH}/%${FILE_TYPE}=${BUILD_PATH}/%.o}
OBJ_FLAGS = -W -Wall -Wextra -Werror ${INCLUDE} -fPIC -std=c++20
BIN_FLAGS = -shared
BIN_NAME = engine.so

all: ${BIN_NAME}

debug: OBJ_FLAGS += -DDEBUG -g3
debug: ${BIN_NAME}

${BUILD_PATH}/%.o: ${SRC_PATH}/%${FILE_TYPE}
	mkdir -p ${dir $@}
	${COMPILER} -MD ${OBJ_FLAGS} -c $< -o $@

${BIN_NAME}: ${OBJ}
	${COMPILER} -o ${BIN_NAME} ${OBJ} ${BIN_FLAGS}

clean:
	rm -rf ${BUILD_PATH}

fclean: clean
	rm -rf ${BIN_NAME}

-include ${OBJ:%.o=%.d}

.PHONY: all clean fclean
