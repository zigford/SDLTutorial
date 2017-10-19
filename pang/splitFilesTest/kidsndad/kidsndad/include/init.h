#pragma once
#ifndef INIT_H_
#define INIT_H_

bool init(bool debug);
void close();
void errorMessage(char* message, bool debug);
char* stringconcat(char* str1, const char* str2);

#endif // !INIT_H_
