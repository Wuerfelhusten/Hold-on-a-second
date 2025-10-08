cmake_minimum_required(VERSION 3.21)
project(HoldOnASec VERSION 1.1.0 LANGUAGES CXX)

# Set your project version and specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Human-friendly plugin name used in metadata and SKSE registry
set(PLUGIN_NAME "Hold on a sec")

# Include SKSEPlugin.cmake from the same directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(SKSEPlugin)

if(MSVC_VERSION GREATER_EQUAL 1936 AND MSVC_IDE) # 17.6+
    file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
    <Project>
      <ItemDefinitionGroup>
        <ClCompile>
          <BuildStlModules>false</BuildStlModules>
        </ClCompile>
      </ItemDefinitionGroup>
    </Project>
    ]==] @ONLY)
endif()