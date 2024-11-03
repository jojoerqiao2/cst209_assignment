CXX := c++
CXXSTD := gnu++23
CXXFLAGS += -std=${CXXSTD} -O2 -Wall -Wextra -Weffc++ -Werror

INCLUDE_DIR := include
SRC_DIR := src

OBJECTS := $(patsubst %.cxx, %.o, $(wildcard ${SRC_DIR}/*.cxx))

all: ${OBJECTS}
	${CXX} ${CXXFLAGS} -I./${INCLUDE_DIR} ${OBJECTS} -o a.out

%.o: %.cxx
	${CXX} ${CXXFLAGS} -I./${INCLUDE_DIR} -c $< -o $@

clean:
	rm -f ${OBJECTS} a.out
