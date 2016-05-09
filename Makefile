test: testing.cpp header.hpp
	clang++ -std=c++14 -otest testing.cpp -I. -I./parser parser/http-parser.c
	
