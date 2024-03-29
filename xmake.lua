package("ftxui")
    set_homepage("https://github.com/ArthurSonzogni/FTXUI")
    set_description(":computer: C++ Functional Terminal User Interface. :heart:")
    set_license("MIT")

    add_urls("https://github.com/ArthurSonzogni/FTXUI/archive/refs/tags/$(version).tar.gz",
             "https://github.com/ArthurSonzogni/FTXUI.git")
    add_versions("v3.0.0", "a8f2539ab95caafb21b0c534e8dfb0aeea4e658688797bb9e5539729d9258cc1")

    add_deps("cmake")

    if is_plat("windows") then
        add_configs("shared", {description = "Build shared library.", default = false, type = "boolean", readonly = true})
    elseif is_plat("bsd") then
        add_syslinks("pthread")
    end

    add_links("ftxui-component", "ftxui-dom", "ftxui-screen")

    on_install("linux", "windows", "macosx", "bsd", "mingw", function (package)
        local configs = {"-DFTXUI_BUILD_DOCS=OFF", "-DFTXUI_BUILD_EXAMPLES=OFF"}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <memory>
            #include <string>

            #include "ftxui/component/captured_mouse.hpp"
            #include "ftxui/component/component.hpp"
            #include "ftxui/component/component_base.hpp"
            #include "ftxui/component/screen_interactive.hpp"
            #include "ftxui/dom/elements.hpp"

            using namespace ftxui;

            void test() {
                int value = 50;
                auto buttons = Container::Horizontal({
                  Button("Decrease", [&] { value--; }),
                  Button("Increase", [&] { value++; }),
                });
                auto component = Renderer(buttons, [&] {
                return vbox({
                           text("value = " + std::to_string(value)),
                           separator(),
                           gauge(value * 0.01f),
                           separator(),
                           buttons->Render(),
                       }) |
                       border;
                });
                auto screen = ScreenInteractive::FitComponent();
                screen.Loop(component);
            }
        ]]}, {configs = {languages = "c++17"}}))
    end)
package_end()

add_rules("mode.debug", "mode.release")
set_languages("cxx17")
add_requires("lua")
add_requires("ftxui")

target("StartUp")
  add_packages("ftxui")
  add_packages("lua")
  set_kind("binary")
  add_includedirs("include", "include/util")
  add_files("src/**.cpp")
