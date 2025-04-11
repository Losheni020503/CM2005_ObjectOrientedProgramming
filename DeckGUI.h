/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "DJLookAndFeel.h"

/**
  * DeckGUI is a class that contains all the components that change the playback of the audio.
  * It includes buttons to play, pause, stop and load an audio file, sliders to change the gain, speed,
  * bass and treble of a track, as well as a label to display the currently playing track and a waveform.
 */
class DeckGUI
    : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::FileDragAndDropTarget {
public:
  
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, PlaylistComponent* playlistComponent,
        juce::Colour colour);

    /**
    * Destructor method used to deallocate memory and cleanup before the object is destroyed
    */
    ~DeckGUI() override;

    //==============================================================================
    // Inherited from Component

    /**
     *  Method used by the component to draw its content
     *
     * @param g the graphics context that must be used to do the drawing operations
     */
    void paint(juce::Graphics& g) override;

    /**
     * Method used to laying out the child components when the width or height of the component changes
     */
    void resized() override;

    //==============================================================================
    // Inherited from Button::Listener

    void buttonClicked(juce::Button* button) override;

    //==============================================================================
    // Inherited from Slider::Listener

    void sliderValueChanged(juce::Slider* slider) override;

    //==============================================================================
    // Inherited from FileDragAndDropTarget

    bool isInterestedInFileDrag(const juce::StringArray& files) override;

   
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    //==============================================================================
    // Inherited from Timer

    void timerCallback() override;

private:
    // Pointer to the playlist component
    PlaylistComponent* playlist;
    // Button to play an audio file
    juce::TextButton playButton;
    // Button to pause an audio file
    juce::TextButton pauseButton;
    // Button to stop and return to the beginning of an audio file
    juce::TextButton stopButton;
    // Button to load a track from the playlist to the deck
    juce::TextButton loadButton;
    // Slider to control volume
    juce::Slider volSlider;
    // Slider to control the room size filter
    juce::Slider roomSizeSlider;
    // Slider to control the Damping level filter
    juce::Slider dampingSlider;
    // Slider to control the speed of the playback
    juce::Slider speedSlider;
    // Slider to control the position of the track
    juce::Slider posSlider;
    // Slider to control the wet level of the playback
    juce::Slider wetLevelSlider;
    // Slider to control the dry level of the playback
    juce::Slider dryLevelSlider;
    // Pointer to the DJAudioPlayer for this deck
    DJAudioPlayer* djAudioPlayer;
    // Label for the track name
    juce::Label trackName;
    // Waveform
    WaveformDisplay waveformDisplay;
    // File chooser object to select files
    juce::FileChooser fChooser{ "Select a file...", juce::File::getSpecialLocation(juce::File::userMusicDirectory),
                               "*.mp3" };
    // Custom LookAndFeel class for buttons and rotary sliders
    DJLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)

};