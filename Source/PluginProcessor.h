#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    The AudioProcessor class handles the audio DSP and parameter state.
*/
class FaithSaturatorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FaithSaturatorAudioProcessor();
    ~FaithSaturatorAudioProcessor() override;

    //==============================================================================
    // DSP Lifecycle Methods
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    // The core audio processing method!
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // Editor (UI) creation
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    // Plugin Metadata block
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    // Preset/Program handling (we keep this minimal as we only have 1 default program)
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    //==============================================================================
    // Save and Load plugin state (parameters) for the DAW session
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Our Parameter Manager
    juce::AudioProcessorValueTreeState apvts;

private:
    // Helper function to define our 3 parameters
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaithSaturatorAudioProcessor)
};
