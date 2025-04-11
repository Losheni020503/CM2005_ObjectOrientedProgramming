/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 * DJAudioPlayer is a class that manages the loading and playing of an audio file
 */
class DJAudioPlayer : public juce::AudioSource {
public:
   
    explicit DJAudioPlayer(juce::AudioFormatManager& _formatManager);

    ~DJAudioPlayer() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    //==============================================================================

    void loadURL(const juce::URL& audioURL);

    void start();

    void pause();

    void stop();

    void setGain(double gain);

    void setSpeed(double ratio);
   
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    double getPositionRelative();

    void setRoomSize(float roomSize);
    void setDamping(float damping);

    void setWetLevel(float wetLevel); // for reverb levels
    void setDryLevel(float dryLevel); // for reverb levels

private:
    // A list of available audio formats
    juce::AudioFormatManager& formatManager;
    // A type of AudioSource that will read from an AudioFormatReader
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // An AudioSource that allows it to be played, stopped, started
    juce::AudioTransportSource transportSource; // to control playback of audio, adjust the volume

    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 }; // wrapper to transport source to control the speed

    juce::ReverbAudioSource reverbSource{ &resampleSource, false };
    juce::Reverb::Parameters reverbParameters;

};