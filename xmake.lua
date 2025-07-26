set_project('donut')
set_version('0.1.0')

set_languages('cxx23')
add_rules('mode.release')

-- options
-- option('interactive')
--   set_default(false)
--   set_showmenu(true)
--   set_description('enables interactive features')
--   add_defines("INTERACTIVE_ENABLED")
-- option_end()

-- option('stats')
-- option_end()

-- targets
target('donut_lib')
  set_kind('static')
  set_basename('donut')
  add_files('src/*.cpp')
  add_includedirs('include', { public = true })
  add_includedirs('include', './external/tomlplusplus/include/', { public = true })
  -- add_headerfiles('include/donut/donut.hpp')

target('donut')
  set_kind('binary')
  set_basename('donut')
  add_files('cli/main.cpp')
  add_includedirs('include', './external/CLI11/include', { public = true })
  add_deps('donut_lib')

target('test')
  set_kind('binary')
  set_basename('test')
  add_files('cli/test.cpp')
  add_deps('donut_lib')
  add_includedirs('include', './external/CLI11/include', { public = true })
  add_includedirs('include', './external/tomlplusplus/include/', { public = true })
