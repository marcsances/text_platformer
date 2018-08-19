#ifndef LOGGER_C
#define LOGGER_C

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>

FILE *_log_out;
FILE *_log_err;
int _logger_is_debug = 0;
int _logger_init = 0;

void logger_init(char *lout, char *lerr, int debug) {
    _log_out = fopen(lout, "a");
    _log_err = fopen(lerr, "a");
    _logger_is_debug = debug;
    _logger_init = 1;
}

void logger_destroy() {
    assert(_logger_init);
    if (!_logger_is_debug) return;
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

void __logger_print(FILE *stream, char *format, va_list vl) {
    assert(_logger_init);
    fprintf(stream, "[%s] ", _curtime());
    vfprintf(stream, format, vl);
    fprintf(stream, "\n");
}

void logger_out(char *format, ...) {
    va_list vl;

    assert(_logger_init);
    va_start(vl, format);
    if (_logger_is_debug) __logger_print(stdout, format, vl);
    va_end(vl);
    va_start(vl, format);
    __logger_print(_log_out, format, vl);
    va_end(vl);
}

void logger_err(char *format, ...) {
    va_list vl;

    assert(_logger_init);
    va_start(vl, format);
    if (_logger_is_debug) __logger_print(stdout, format, vl);
    va_end(vl);
    va_start(vl, format);
    __logger_print(_log_err, format, vl);
    va_end(vl);
}

#endif
