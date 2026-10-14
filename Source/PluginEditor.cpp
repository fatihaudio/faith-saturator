#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FaithSaturatorAudioProcessorEditor::FaithSaturatorAudioProcessorEditor (FaithSaturatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Define window size for our UI
    setSize (400, 300);

    // 1. Setup Drive Slider
    driveSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible (driveSlider);
    
    // Label for Drive Slider
    driveLabel.setText ("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType (juce::Justification::centred);
    driveLabel.attachToComponent (&driveSlider, false); 
    addAndMakeVisible (driveLabel);

    // 2. Setup Mix Slider
    mixSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible (mixSlider);

    // Label for Mix Slider
    mixLabel.setText ("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType (juce::Justification::centred);
    mixLabel.attachToComponent (&mixSlider, false);
    addAndMakeVisible (mixLabel);

    // 3. Setup Output Range Slider
    outputSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible (outputSlider);

    // Label for Output Slider
    outputLabel.setText ("Output", juce::dontSendNotification);
    outputLabel.setJustificationType (juce::Justification::centred);
    outputLabel.attachToComponent (&outputSlider, false);
    addAndMakeVisible (outputLabel);

    // 4. Attach the Sliders to the APVTS Parameters in the Processor
    // This connects the UI to the actual DSP values safely!
    driveAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DRIVE", driveSlider);
    mixAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "MIX", mixSlider);
    outputAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "OUTPUT", outputSlider);
}

FaithSaturatorAudioProcessorEditor::~FaithSaturatorAudioProcessorEditor()
{
}

//==============================================================================
void FaithSaturatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background with a solid color
    g.fillAll (juce::Colours::darkgrey);

    // Draw a basic title explicitly
    g.setColour (juce::Colours::white);
    g.setFont (30.0f);
    g.drawFittedText ("Faith Saturator", getLocalBounds().removeFromTop(50), juce::Justification::centred, 1);
}

void FaithSaturatorAudioProcessorEditor::resized()
{
    // Layout the 3 knobs nicely in a row using Juce FlexBox or simple bounds maths
    auto area = getLocalBounds().reduced(20);
    
    // Reserve the top for the title text we just painted
    area.removeFromTop(40);

    // Calculate space for 3 columns
    auto knobWidth = area.getWidth() / 3;
    
    // Assign sizes and positions to our sliders (the labels will follow automatically because of attachToComponent)
    driveSlider.setBounds(area.removeFromLeft(knobWidth).reduced(10));
    mixSlider.setBounds(area.removeFromLeft(knobWidth).reduced(10));
    outputSlider.setBounds(area.removeFromLeft(knobWidth).reduced(10));
}
