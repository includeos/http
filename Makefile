test: testing.cpp header.hpp
	clang++ -std=c++14 -otest testing.cpp -I. -Iparser parser/http-parser.c
	
