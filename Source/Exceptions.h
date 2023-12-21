#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

struct SintaxError : std::runtime_error {
	SintaxError(const char* msg) : std::runtime_error(msg) { }
	SintaxError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct LexicalError : std::runtime_error {
	LexicalError(const char* msg) : std::runtime_error(msg) { }
	LexicalError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct ParseError : std::runtime_error {
	ParseError(const char* msg) : std::runtime_error(msg) { }
	ParseError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct SemanticError : std::runtime_error {
	SemanticError(const char* msg) : std::runtime_error(msg) { }
	SemanticError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

struct InputError : std::runtime_error {
	InputError(const char* msg) : std::runtime_error(msg) { }
	InputError(std::string msg) : std::runtime_error(msg.c_str()) { }
};

#endif