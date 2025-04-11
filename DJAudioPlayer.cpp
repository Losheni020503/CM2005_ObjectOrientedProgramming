/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    reverbParameters.roomSize = 0.0f; // default setting for reverb; range of 0 - 1, where 1 is big
    reverbParameters.damping = 0.0f; // default setting for reverb; range of 0 - 1, where 1 is full damped
    reverbParameters.wetLevel = 0.0f; // default setting for reverb; range of 0 - 1
    reverbParameters.dryLevel = 1.0f; // default setting for reverb; range of 0 - 1
    reverbSource.setParameters(reverbParameters);
}  

DJAudioPlayer::~DJAudioPlayer() = default;

//==============================================================================

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) 
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    //transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources(); 
}

//==============================================================================

void DJAudioPlayer::loadURL(const juce::URL& audioURL) {
    auto* reader = formatManager.createReaderFor(
        audioURL.createInputStream(juce::URL::InputStreamOptions{ juce::URL::ParameterHandling::inAddress }));

    if (reader == nullptr) {
        return;
    }

    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{

    if (ratio < 0 || ratio > 100)
    {
        std::cout << "speed should be between 0 and 1" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{

    if (pos < 0 || pos > 1.0)
    {
        std::cout << "positon should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = pos * transportSource.getLengthInSeconds();
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setRoomSize(float size)
{
    DBG("setRoomSize called");
    if (size < 0 || size > 1.0)
    {
        DBG("DJAudioPlayer::setRoomSize size should be between 0 and 1.0");
    }
    else {
        reverbParameters.roomSize = size;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDamping(float dampingAmt)
{
    DBG("setDamping called");
    if (dampingAmt < 0 || dampingAmt > 1.0)
    {
        DBG("DJAudioPlayer::setDamping amount should be between 0 and 1.0");
    }
    else {
        reverbParameters.damping = dampingAmt;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setWetLevel(float wetLevel)
{
    DBG("setWetLevel called");
    if (wetLevel < 0 || wetLevel > 1.0)
    {
        DBG("setWetLevel level should be between 0 and 1.0");
    }
    else {
        reverbParameters.wetLevel = wetLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::setDryLevel(float dryLevel)
{
    DBG("setDryLevel called");
    if (dryLevel < 0 || dryLevel > 1.0)
    {
        DBG("setDryLevel level should be between 0 and 1.0");
    }
    else {
        reverbParameters.dryLevel = dryLevel;
        reverbSource.setParameters(reverbParameters);
    }
}

void DJAudioPlayer::start() {
    transportSource.start();
}

void DJAudioPlayer::pause() {
    transportSource.stop();
}

void DJAudioPlayer::stop() {
    pause();
    setPosition(0);
}
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}