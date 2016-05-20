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
		src/message.cpp src/header.cpp src/span.cpp

OBJECTS=request.o response.o version.o message.o header.o span.o

DEP=inc/parser/http_parser.cpp

test: test.cpp objs
	${CXX} ${CXXFLAGS} ${INCLUDES} -otest test.cpp ${OBJECTS} ${DEP}

lib: objs
	ar -cq libhttp.a ${OBJECTS}
	ranlib libhttp.a
	mkdir lib
	mv libhttp.a lib
	rm *.o
	
objs: src/request.cpp src/response.cpp src/message.cpp src/header.cpp src/span.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c ${SOURCES}

clean:
	rm *.o test
