#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

//==============================================================================
/**
    The AudioProcessorEditor handles the user interface (UI) for the plugin.
*/
class FaithSaturatorAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  //==============================================================================
  FaithSaturatorAudioProcessorEditor(FaithSaturatorAudioProcessor &);
  ~FaithSaturatorAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FaithSaturatorAudioProcessor &audioProcessor;

  //==============================================================================
  // The three sliders for our parameters
  juce::Slider driveSlider;
  juce::Slider mixSlider;
  juce::Slider outputSlider;

  // The labels to accompany the sliders
  juce::Label driveLabel;
  juce::Label mixLabel;
  juce::Label outputLabel;

  // Define aliases to keep the code neat
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

  // These attachments link our UI sliders to the DSP parameters automatically
  std::unique_ptr<SliderAttachment> driveAttachment;
  std::unique_ptr<SliderAttachment> mixAttachment;
  std::unique_ptr<SliderAttachment> outputAttachment;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      FaithSaturatorAudioProcessorEditor)
};
