cmake_minimum_required(VERSION 3.24)

# Define the project name and version
project(FaithSaturator VERSION 1.0.0)

# We want modern C++ for JUCE
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure macOS deployment target
if (APPLE)
    # Target a modern macOS version (e.g., 10.15 gives good C++20 support natively)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "macOS deployment target")
    # Specifically target Apple Silicon (arm64) as requested
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "macOS Architectures")
endif()

# Fetch JUCE from GitHub automatically
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.6 # The latest stable release
)
FetchContent_MakeAvailable(juce)

# Create the Audio Plugin (VST3 and AU formats)
juce_add_plugin(FaithSaturator
    PLUGIN_MANUFACTURER_CODE "Ftbl"   # 4 character ID for the manufacturer
    PLUGIN_CODE "Fsat"                # 4 character ID for the plugin
    FORMATS VST3 AU                   # Target VST3 and Audio Unit formats
    PRODUCT_NAME "Faith Saturator"
    
    # Plugin configuration (It's an effect, not an instrument)
    IS_SYNTH FALSE                     
    NEEDS_MIDI_INPUT FALSE             
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    
    # Automatically copy to the local plugin folders (~/Library/Audio/Plug-Ins) after build
    COPY_PLUGIN_AFTER_BUILD TRUE       
)

# Generate JuceHeader.h for our plugin
juce_generate_juce_header(FaithSaturator)

# Add our future source files to the plugin target
target_sources(FaithSaturator
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Compile-time definitions for the plugin
target_compile_definitions(FaithSaturator
    PUBLIC
        # Disable unused JUCE modules and legacy formats
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link against the necessary JUCE modules
target_link_libraries(FaithSaturator
    PRIVATE
        juce::juce_audio_plugin_client
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)
