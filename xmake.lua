set_project('donut')
set_version('0.1.0')

set_languages('cxx23')
add_rules('mode.release')

-- donut core library
target('donut_lib')
    set_kind('static')
    set_basename('donut')
    add_files('src/*.cpp')
    add_headerfiles('include/(donut/*.hpp)')
    add_includedirs('include', { public = true })
    add_includedirs('include', './external/tomlplusplus/include/', { public = true })

-- basic donut binary
target('donut_basic')
    set_kind('binary')
    set_basename('donut')
    add_files('cli/main.cpp')
    add_deps('donut_lib')

target('test')
    set_kind('binary')
    set_basename('test')
    add_files('cli/test.cpp')
    add_deps('donut_lib')
    add_includedirs('include', './external/CLI11/include', { public = true })
    add_includedirs('include', './external/tomlplusplus/include/', { public = true })
