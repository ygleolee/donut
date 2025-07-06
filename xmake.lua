---@diagnostic disable: undefined-global
set_project("donut")
set_version("0.1.0")

-- Global config
set_languages("cxx20")
add_rules("mode.debug", "mode.release")


-- donut core library
target("donut_lib")
    set_kind("static")
    set_basename("donut")
    add_files("src/*.cpp")
    add_headerfiles("include/(donut/*.hpp)")
    add_includedirs("include", {public = true})

-- basic donut binary
target("donut_basic")
    set_kind("binary")
    set_basename("donut")
    add_files("binaries/main.cpp")
    add_deps("donut_lib")  -- link to donut library

-- test that CLI11 works
target("test_CLI")
    set_kind("binary")
    set_basename("test")
    add_files("binaries/test_CLI.cpp")
    add_includedirs("include", "external/CLI11/include", {public = true})
