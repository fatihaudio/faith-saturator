#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FaithSaturatorAudioProcessor::FaithSaturatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
// Initialize the APVTS with our parameter layout
apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

FaithSaturatorAudioProcessor::~FaithSaturatorAudioProcessor()
{
}

//==============================================================================
// Define the audio parameters (Drive, Mix, Output)
juce::AudioProcessorValueTreeState::ParameterLayout FaithSaturatorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Drive parameter (0.0 to 10.0, default 1.0)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"DRIVE", 1}, "Drive", 0.0f, 10.0f, 1.0f));

    // Mix parameter (0.0 to 1.0, default 1.0 / 100% wet)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"MIX", 1}, "Mix", 0.0f, 1.0f, 1.0f));

    // Output gain parameter (-24dB to +24dB, default 0.0dB)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"OUTPUT", 1}, "Output", -24.0f, 24.0f, 0.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
void FaithSaturatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void FaithSaturatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

//==============================================================================
// The DSP loop
void FaithSaturatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear out any garbage memory on buses we aren't using
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Fetch the current parameter values from the APVTS
    const float driveValue  = apvts.getRawParameterValue("DRIVE")->load();
    const float mixValue    = apvts.getRawParameterValue("MIX")->load();
    const float outputValue = apvts.getRawParameterValue("OUTPUT")->load();
    
    // Convert output gain from dB to a linear multiplier
    const float outputLinear = juce::Decibels::decibelsToGain(outputValue);

    // Iterate over the audio channels (left/right for stereo)
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // Iterate over the individual samples in the buffer block
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            
            // 1. Apply Drive (boost signal before clipping)
            // We add 1.0 so when drive is 0, multiplier is 1.0 (no change)
            float drivenSample = inputSample * (1.0f + driveValue);
            
            // 2. Apply Soft Clipping (tanh is a common, safe S-curve function for soft clipping)
            // It safely rounds off peaks instead of hard chopping them.
            float wetSample = std::tanh(drivenSample);
            
            // 3. Apply Mix (Dry/Wet blend)
            // Linear interpolation between the dry unprocessed input and the distorted wet signal
            float mixedSample = (inputSample * (1.0f - mixValue)) + (wetSample * mixValue);
            
            // 4. Apply Output Gain and write the sample back to the buffer
            channelData[sample] = mixedSample * outputLinear;
        }
    }
}

//==============================================================================
// Setup UI connection
juce::AudioProcessorEditor* FaithSaturatorAudioProcessor::createEditor()
{
    return new FaithSaturatorAudioProcessorEditor (*this);
}

//==============================================================================
// Save parameters to DAW state
void FaithSaturatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

// Load parameters from DAW state
void FaithSaturatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FaithSaturatorAudioProcessor();
}
