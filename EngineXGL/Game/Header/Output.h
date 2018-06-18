#pragma once

#include <string>

void exOutput(const char* text);

template <typename T>
void exOutputTransform(T text)
{
	std::string temp(std::to_string(text) + "\n");
	const char* finalChar = temp.c_str();
	exOutput(finalChar);
}

