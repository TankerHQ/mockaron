from conans import ConanFile, CMake, tools


class MockaronConan(ConanFile):
    name = "mockaron"
    version = "0.2"
    license = "BSD-2-Clause"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = "shared=False", "fPIC=True"
    description = "A C++ mocking library"
    repo_url = "https://github.com/TankerHQ/mockaron"
    generators = "cmake"
    exports_sources = "*"

    @property
    def should_build_tests(self):
        develop = self.develop
        cross_building = tools.cross_building(self.settings)
        emscripten = self.settings.os == "Emscripten"
        return develop and (not cross_building) and (not emscripten)

    def build(self):
        cmake = CMake(self)
        if not self.should_build_tests:
            cmake.definitions["MOCKARON_NOTEST"] = "ON"
        if self.options.fPIC:
            cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = "ON"
        cmake.configure()
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("*COPYING", dst="licenses", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["mockaron"]
