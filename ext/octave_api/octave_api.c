#include "octave_api.h"
#include "ruby.h"
#include "octave-ruby.h"

// Defining a space for information and references about the module to be stored internally
VALUE OCTAVE_API = Qnil;

// The initialization method for this module
void Init_octave_api() {
  OCTAVE_API = rb_define_module("Octave");
  OCTAVE_API = rb_define_module_under(OCTAVE_API, "Driver");
  OCTAVE_API = rb_define_module_under(OCTAVE_API, "Native");
  OCTAVE_API = rb_define_module_under(OCTAVE_API, "API");
  rb_gc_unregister_address(&OCTAVE_API);
  rb_define_module_function(OCTAVE_API, "feval", feval, 2);
  rb_define_module_function(OCTAVE_API, "get_variable", get_variable, 1);
  rb_define_module_function(OCTAVE_API, "put_variable", put_variable, 2);
  rb_define_module_function(OCTAVE_API, "exit", octave_exit, 0);
  initialize_octave();
}

VALUE feval(VALUE self, VALUE function_name, VALUE arguments)
{
  return or_feval(function_name, arguments);
}

static VALUE get_variable(VALUE self, VALUE variable_name)
{
  return or_get_variable(variable_name);
}

static VALUE put_variable(VALUE self, VALUE variable_name, VALUE value)
{
  return or_put_variable(variable_name, value);
}

static VALUE octave_exit(VALUE self)
{
  return or_exit();
}
