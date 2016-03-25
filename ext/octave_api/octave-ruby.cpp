#include "octave-ruby.h"
#include "octave-includes.h"
#include "or-variable.h"

void initialize_octave()
{
  /* this should really be const char * --thanks, Octave */
  char *argv[4];
  argv[0] = (char *) "octave-ruby";
  argv[1] = (char *) "-q";  /* quiet! */
  argv[2] = (char *) "-H";  /* no history file */
  argv[3] = (char *) "-f";  /* ignore .rc files */
  octave_main(4,argv,1);
  set_global_value("crash_dumps_octave_core", false);
}

extern void recover_from_exception(void)
{
  // unwind_protect::run_all();
  can_interrupt = true;
  octave_interrupt_immediately = 0;
  octave_interrupt_state = 0;
  octave_exception_state = octave_no_exception;
}

VALUE or_feval(VALUE function_name, VALUE arguments)
{
  VALUE ruby_val = Qnil;
  int i, n;
  octave_value_list argList;
  
  n = RARRAY_LEN(arguments);
  
  bool is_function_definition = (n == 1 && FIXNUM_P(RARRAY_PTR(arguments)[0]) == 0 && strncmp(RSTRING_PTR(StringValue(RARRAY_PTR(arguments)[0])), "function ", 9) == 0);
  
  for (i = 0; i < n; i++) {
    argList(i) = OR_Variable(RARRAY_PTR(arguments)[i]).to_octave();
  }
  
  if (octave_set_current_context) {
    // unwind_protect::run_all();
    raw_mode(0);
  }

  can_interrupt = true;
  octave_initialized = true;

  try {
    symbol_table::set_scope(symbol_table::top_scope());
    reset_error_handler();
    
    int nargout = (is_function_definition ? 0 : 1);
    octave_value_list val = feval(std::string(RSTRING_PTR(function_name)), argList, nargout);
    if(val.length() > 0 && val(0).is_defined()) {
      ruby_val = OR_Variable(val(0)).to_ruby();
    }
  } catch (octave_interrupt_exception) {
    recover_from_exception();
    error_state = -2; 
  } catch (std::bad_alloc) {
    recover_from_exception();
    error_state = -3;
  }

  octave_initialized = false;

  return(ruby_val);
}

extern VALUE or_get_variable(VALUE variable_name)
{
  return OR_Variable(get_top_level_value(std::string(RSTRING_PTR(variable_name)))).to_ruby();
}

extern VALUE or_put_variable(VALUE variable_name, VALUE value)
{
  set_top_level_value(std::string(RSTRING_PTR(variable_name)), OR_Variable(value).to_octave());
  return value;
}

extern VALUE or_exit()
{
  clean_up_and_exit(0, 1);
  return Qnil;
}
