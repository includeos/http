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

CXX=clang++
CXXFLAGS=-std=c++14 -Wall -Wextra -Ofast
INCLUDES=-I./inc -I./inc/parser

SOURCES=src/request.cpp src/response.cpp src/version.cpp \
		src/message.cpp src/header.cpp src/header_fields.cpp src/span.cpp src/time.cpp

OBJECTS=request.o response.o version.o message.o header.o header_fields.o span.o time.o

DEP=inc/parser/http_parser.cpp
DEP_OBJ=http_parser.o

test: test.cpp objs
	${CXX} ${CXXFLAGS} ${INCLUDES} -otest test.cpp ${OBJECTS} ${DEP_OBJ}

lib: objs
	ar -cq libhttp.a ${OBJECTS} ${DEP_OBJ}
	ranlib libhttp.a
	mkdir lib
	mv libhttp.a lib
	rm *.o
	
objs: src/request.cpp src/response.cpp src/message.cpp src/header.cpp src/span.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c ${SOURCES} ${DEP}

clean:
	rm *.o test
