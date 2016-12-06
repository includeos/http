# This file is a part of the IncludeOS unikernel - www.includeos.org
#
# Copyright 2015-2016 Oslo and Akershus University College of Applied Sciences
# and Alfred Bratterud
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Compiler Settings #
CXX=clang++

# Compiler Flag Settings #
CXXFLAGS=-std=c++14 -Wall -Wextra -O3

# Header File Locations #
INCLUDES=-I./inc -I./inc/parser

# Source Code Locations #
CPP_SOURCES=${wildcard src/*.cpp}
CPP_SOURCES+=${wildcard inc/parser/*.cpp}

# Object Files #
OBJECTS=${CPP_SOURCES:.cpp=.o}

LIB=lib/libhttp.a

lib: ${OBJECTS}
	mkdir -p lib
	ar -cq ${LIB} ${OBJECTS}
	ranlib ${LIB}

test: test.cpp lib
	${CXX} ${CXXFLAGS} ${INCLUDES} -otest test.cpp ${LIB}

%.o: %.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

clean:
	${RM} ${OBJECTS}
	${RM} -rf lib/
	${RM} test
