/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "STR-X.h"

//==============================================================================
/**
*/
class STRXAudioProcessor  : public AudioProcessor,
                            public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    STRXAudioProcessor();
    ~STRXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged(const String& parameterID, float newValue) override;
    
    void updateOversample();
    
    void updateFilters();

    AudioProcessorValueTreeState apvts;

    std::array<dsp::Oversampling<float>, 3> oversample
    { {
        {dsp::Oversampling<float>(2)},
        {dsp::Oversampling<float>(2)},
        {dsp::Oversampling<float>(2)},
    } };
    
    int lastUIWidth, lastUIHeight;

private:

    int osIndex = 0;
    double lastSampleRate = 0.0;
    double lastDownSampleRate = 0.0;
    int numSamples = 0.0;

    bool isOversampled = false;

    AudioProcessorValueTreeState::ParameterLayout createParameters();

    NormalisableRange<float> nRange, outVolRange;

    std::atomic<float>* hq = nullptr, *renderHQ = nullptr, *outVol_dB = nullptr, *legacyTone = nullptr;

    AmpProcessor amp;

    /*input * (max-min)+min*/
    float cookParams(float valueToCook, float minValue, float maxValue) 
    {
        return valueToCook * (maxValue - minValue) + minValue;
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (STRXAudioProcessor)
};
