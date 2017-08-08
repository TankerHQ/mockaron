from conans import tools, CMake, ConanFile
import os


class MockaronConan(ConanFile):
    name = "mockaron"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports = "CMakeLists.txt", "test*"
    exports_sources = "src*", "include*"

    @property
    def mockaron_src(self):
        return os.path.join(self.source_folder, "src")

    def build(self):
        self.cmake_helper = CMake(self)
        if self.options["shared"]:
            self.cmake_helper.definitions["BUILD_SHARED_LIBS"] = "ON"
        self.cmake_helper.configure()
        self.cmake_helper.build()

    def package(self):
        self.cmake_helper.install()

    def package_info(self):
        self.cpp_info.libs = ["mockaron"]
