#ifndef LOGGER_C
#define LOGGER_C
#endif

#include <stdio.h>
#include <time.h>
#include <assert.h>

FILE *_log_out;
FILE *_log_err;
int _logger_is_debug = 0;
int _logger_init = 0;

void logger_init(char *lout, char *lerr, int debug) {
    _log_out = fopen(lout, "w");
    _log_err = fopen(lerr, "w");
    _logger_is_debug = debug;
    _logger_init = 1;
}

void logger_destroy() {
    assert(_logger_init);
    fclose(_log_out);
    fclose(_log_err);
}

char *_curtime() {
    time_t rawtime;
    struct tm * timeinfo;
    char *stime;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    stime = asctime (timeinfo);
    stime[strlen(stime)-1] = '\0';
    return stime;
}

void __logger_print(FILE *cstream, FILE *fstream, char *text) {
    assert(_logger_init);
    if (_logger_is_debug) {
        fprintf(cstream,"[%s] %s\n",_curtime(),text);
    } 
    fprintf(fstream,"[%s] %s\n",_curtime(),text);
}

void logger_out(char *text) {
    assert(_logger_init);
    __logger_print(stdout, _log_out, text);
}

void logger_err(char *text) {
    assert(_logger_init);
    __logger_print(stderr, _log_err, text);
}