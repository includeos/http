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

CXX=clang++-3.8
CXXFLAGS=-std=c++14 -Wall -Wextra -Ofast
INCLUDES=-I./inc -I./inc/parser

SOURCES=${wildcard src/*.cpp} inc/parser/http_parser.cpp
OBJECTS=${SOURCES:.cpp=.o}
LIB=lib/libhttp.a


test: test.cpp ${OBJECTS}
	${CXX} ${CXXFLAGS} ${INCLUDES} -o test test.cpp lib/libhttp.a

lib: ${OBJECTS}
	mkdir -p lib
	ar -cq $(LIB) ${OBJECTS}
	ranlib $(LIB)
	$(RM) $(OBJECTS)

%.o: %.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@


install:
	mkdir -p ${INCLUDEOS_INSTALL}/packages/include/http
	mkdir -p ${INCLUDEOS_INSTALL}/packages/lib/http
	cp -r inc/* ${INCLUDEOS_INSTALL}/packages/include/http
	cp -r lib/* ${INCLUDEOS_INSTALL}/packages/lib/http


clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
	$(RM) test
