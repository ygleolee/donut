set_project("donut")
set_version("0.1.0")

-- Global config
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- Add CLI11 headers
-- add_includedirs("include", "external/CLI11/include", {public = true})

-- Donut core library
target("libdonut")
    set_kind("static")  -- or "shared"
    add_files("src/*.cpp")
    add_headerfiles("include/(donut/*.hpp)")
    add_includedirs("include", {public = true})

-- CLI binary
target("donut")
    set_kind("binary")
    add_files("binaries/main.cpp")
    add_deps("libdonut")  -- link to donut library
