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
    exports_sources = "CMakeLists.txt", "src", "include", "test", "COPYING"

    @property
        tools.get(zip_url, sha256=expected_hash)

    def build(self):

        cmake = CMake(self)
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
