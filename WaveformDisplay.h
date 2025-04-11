/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

/**
 *  WaveformDisplay is a class used to create and display a waveform of the overall loudness
 */
class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
    /* Constructor method used to initialize data members */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse, DJAudioPlayer* djAudioPlayer, juce::Colour colour);

    /* Destructor method used to deallocate memory and cleanup before the object is destroyed */
    ~WaveformDisplay() override;

    //==============================================================================
    // Inherited by Component

    /* Method used by the component to draw its content */
    void paint(juce::Graphics& g) override;

    /* Method used to laying out the child components when the width or height of the component changes */
    void resized() override;

    /* Mouse event called when the mouse button is pressed */
    void mouseDown(const juce::MouseEvent& event) override;

    /* Mouse event called when the mouse is moved while a button is held down */
    void mouseDrag(const juce::MouseEvent& event) override;

    //==============================================================================
    // Inherited by ChangeListener

    /* Method that receives change event callbacks */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    //==============================================================================

    /* Method to load and generate a new thumbnail from an URL */
    void loadURL(const juce::URL& audioURL);

    /* Method to set the relative position of the indicator */
    void setPositionRelative(double pos);

private:
    // Object to quickly draw scaled views of the waveform shape of an audio file.
    juce::AudioThumbnail audioThumbnail;
    // Flag to indicate that a thumbnail has been created
    bool fileLoaded;
    // Position of an indicator of the current location in an audio file
    double position;
    // Pointer to its corresponding DJAudioPlayer
    DJAudioPlayer* djAudioPlayer;

    // Colour of the waveform display
    juce::Colour colour;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};