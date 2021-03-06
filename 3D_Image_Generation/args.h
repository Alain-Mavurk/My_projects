#ifndef __ARGS_H__
#define __ARGS_H__
typedef struct option_s {
  const char* keyword;
  enum { OptVoid, OptInt, OptString, OptFloat } spec;
  union {
    void (*opt_void)();
    void (*opt_int)(int);
    void (*opt_str)(const char*);
    void (*opt_float)(float);
  } fct;
  struct option_s* next;
} option_t;


option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_int(option_t* l, const char* kw, void (*f)(int));
option_t* opt_float(option_t* l, const char* kw, void (*f)(float));
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
void opt_delete(option_t* l);
void process_options(option_t* l, int argc, char* *argv);

#endif