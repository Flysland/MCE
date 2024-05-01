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
	${SRC_PATH}/main.cpp\

INCLUDE =\
	-I./include/\

OBJ = ${SRC:${SRC_PATH}/%${FILE_TYPE}=${BUILD_PATH}/%.o}
OBJ_FLAGS = -g3 -W -Wall -Wextra -Werror ${INCLUDE} -std=c++20
BIN_FLAGS =
BIN_NAME = tests_run

all: ${BIN_NAME}

${BUILD_PATH}/%.o: ${SRC_PATH}/%${FILE_TYPE}
	mkdir -p ${dir $@}
	${COMPILER} -MD ${OBJ_FLAGS} -c $< -o $@

${BIN_NAME}: ${OBJ}
	${COMPILER} -L../ -o ${BIN_NAME} ${OBJ} ${BIN_FLAGS} -lengine

clean:
	rm -rf ${BUILD_PATH}

fclean: clean
	rm -rf ${BIN_NAME}

-include ${OBJ:%.o=%.d}

.PHONY: all clean fclean
