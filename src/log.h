//
// Created by qwerty on 18. 4. 2017.
//

#ifndef STELNET_LOG_H
#define STELNET_LOG_H

extern bool logging_enabled;

void printdata(const char *tag, void *c, unsigned long n);
void printdatahex(const char *tag, char *c, long n);


#endif //STELNET_LOG_H
