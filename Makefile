test: testing.cpp header.hpp
	git submodule init
	git submodule update
	clang++ -std=c++14 -otest testing.cpp -I. -Ihttp-parser http-parser/http-parser.c
	