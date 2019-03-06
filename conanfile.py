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
    exports_sources = "*.patch"

    @property
    def mockaron_src(self):
        return "%s-%s" % (self.name, self.version)

    def source(self):
        zip_url = "%s/archive/v%s.zip" % (self.repo_url, self.version)
        expected_hash = "2f1e28619b8a705432eebb9e1ef41004f04eb5ee38d31afdef3c23deb7f24926"
        tools.get(zip_url, sha256=expected_hash)

    def build(self):
        tools.patch(patch_file="CMakeLists.txt.patch", base_path=self.mockaron_src)

        cmake = CMake(self)
        cmake.definitions["MOCKARON_NOTEST"] = "ON"
        if self.options.fPIC:
            cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = "ON"
        cmake.configure(source_dir=self.mockaron_src)
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("*COPYING", dst="licenses", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["mockaron"]
