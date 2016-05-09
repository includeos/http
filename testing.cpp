#include <iostream>
#include <header.hpp>

#include <http_parser.h>

http::Header headers;

span field;
span value;

int on_field(http_parser* parser, const char* at, size_t length) {
	(void)parser;
	field.data = at;
	field.len = length;
	return 0;
}

int on_value(http_parser* parser, const char* at, size_t length) {
	(void)parser;
	value.data = at;
	value.len = length;

	headers.add_field(field, value);
	return 0;
}
int main() {

	http_parser* parser = (http_parser*)malloc(sizeof(http_parser));
	http_parser_init(parser, HTTP_REQUEST);

	http_parser_settings settings;
	http_parser_settings_init(&settings);
	settings.on_header_field = on_field;
	settings.on_header_value = on_value;

	const char* request = "GET http://include.os:5000/index.html?install=x86_64 HTTP/1.1\r\n"
	                      "Host: IncludeOS\r\n"
	                      "Accept: text/html\r\n\r\n"
	                      "Something to test with";

	http_parser_execute(parser, &settings, request, strlen(request));

	std::cout << headers;
}