/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, PlaylistComponent* playlistComponent,
    juce::Colour colour)
    : djAudioPlayer{ player },
    waveformDisplay(formatManager, cache, player, colour),
    playlist(playlistComponent),
    customLookAndFeel{ colour } {
    // General LookAndFeel
    setLookAndFeel(&customLookAndFeel);

    // Initialize Buttons
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setButtonText("Play");

    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    pauseButton.setButtonText("Pause");

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setButtonText("Stop");

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setButtonText("Load");

    // Initialize Sliders
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setName("Volume");
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5f);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, volSlider.getTextBoxWidth()/2, volSlider.getTextBoxHeight());

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setName("Speed");
    speedSlider.setRange(0.0, 10);
    speedSlider.setValue(1.0f);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, speedSlider.getTextBoxWidth()/2, speedSlider.getTextBoxHeight());

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setName("Position");
    posSlider.setRange(0.0f, 1.0f, 0.0001f);
    posSlider.setValue(0.03);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, posSlider.getTextBoxWidth() / 2, posSlider.getTextBoxHeight());

    addAndMakeVisible(dampingSlider);
    dampingSlider.addListener(this);
    dampingSlider.setName("Damping");
    dampingSlider.setRange(0.0f, 1.0f);
    dampingSlider.setValue(0.0f);
    dampingSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, dampingSlider.getTextBoxWidth(), dampingSlider.getTextBoxHeight());

    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.addListener(this);
    roomSizeSlider.setName("Room Size");
    roomSizeSlider.setRange(0.0f, 1.0f);
    roomSizeSlider.setValue(0.0f);
    roomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, roomSizeSlider.getTextBoxWidth(), roomSizeSlider.getTextBoxHeight());

    addAndMakeVisible(wetLevelSlider);
    wetLevelSlider.addListener(this);
    wetLevelSlider.setName("Wet Level");
    wetLevelSlider.setRange(0.0f, 1.0f);
    wetLevelSlider.setValue(0.0f);
    wetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, wetLevelSlider.getTextBoxWidth(), wetLevelSlider.getTextBoxHeight());

    addAndMakeVisible(dryLevelSlider);
    dryLevelSlider.addListener(this);
    dryLevelSlider.setName("Dry Level");
    dryLevelSlider.setRange(0.0f, 1.0f);
    dryLevelSlider.setValue(1.0f);
    dryLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, dryLevelSlider.getTextBoxWidth(), dryLevelSlider.getTextBoxHeight());

    // Initialize Track Name
    addAndMakeVisible(trackName);
    trackName.setText("No track", juce::dontSendNotification);
    trackName.setFont(juce::Font(32.0f));

    // Initialize Waveform
    addAndMakeVisible(waveformDisplay);

    startTimer(500);
}

DeckGUI::~DeckGUI() {
    setLookAndFeel(nullptr);
    stopTimer();
}

//==============================================================================

void DeckGUI::paint(juce::Graphics& g) {
    // Clear the background
//    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);

    // Draw an outline around the component
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

void DeckGUI::resized() {
    auto area = getLocalBounds();
    auto rowHeight = getHeight() / 8;

    trackName.setBounds(area.removeFromTop(rowHeight));
    waveformDisplay.setBounds(area.removeFromTop(2 * rowHeight));

    auto width = getWidth() / 7;

    auto sliderArea = area.removeFromTop(4 * rowHeight);
    roomSizeSlider.setBounds(sliderArea.removeFromLeft(width).reduced(10));
    dampingSlider.setBounds(sliderArea.removeFromLeft(width).reduced(10));
    wetLevelSlider.setBounds(sliderArea.removeFromLeft(width).reduced(10));
    dryLevelSlider.setBounds(sliderArea.removeFromLeft(width).reduced(10));
    posSlider.setBounds(sliderArea.removeFromLeft(width).reduced(15, 5));
    volSlider.setBounds(sliderArea.removeFromLeft(width).reduced(15, 5));
    speedSlider.setBounds(sliderArea.removeFromLeft(width).reduced(15, 5));

    auto buttonArea = area.removeFromTop(rowHeight);
    playButton.setBounds(buttonArea.removeFromLeft(width));
    pauseButton.setBounds(buttonArea.removeFromLeft(width));
    stopButton.setBounds(buttonArea.removeFromLeft(width));
    loadButton.setBounds(buttonArea.removeFromLeft(width));
}

//==============================================================================

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        djAudioPlayer->start();
    }
    if (button == &pauseButton) {
        djAudioPlayer->pause();
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
    }
    if (button == &loadButton) {
        auto track = playlist->getFirstSelectedTrack();
        // If a track couldn't be loaded, just exit
        if (track.getDuration() == -1) {
            return;
        }
        auto audioURL = track.getAudioURL();
        this->djAudioPlayer->loadURL(audioURL);
        this->waveformDisplay.loadURL(audioURL);
        this->trackName.setText(track.getTitle(), juce::dontSendNotification);
    }
}

//==============================================================================

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &dampingSlider) {
        djAudioPlayer->setDamping(slider->getValue());
    }
    if (slider == &volSlider) {
        djAudioPlayer->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        djAudioPlayer->setSpeed(slider->getValue());
    }
    if (slider == &posSlider){
        djAudioPlayer->setPositionRelative(slider->getValue());
    }
    if (slider == &roomSizeSlider) {
        djAudioPlayer->setRoomSize(slider->getValue());
    }
    if (slider == &wetLevelSlider)
    {
        djAudioPlayer->setWetLevel(slider->getValue());
    }
    if (slider == &dryLevelSlider)
    {
        djAudioPlayer->setDryLevel(slider->getValue());
    }
    
}

//==============================================================================

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {

    for (auto i = 0; i < files.size(); i++) {
        auto file = juce::File{ files[i] };
        auto audioURL = juce::URL{ file };
        if (i == 0) {
            djAudioPlayer->loadURL(audioURL);
            waveformDisplay.loadURL(audioURL);
            trackName.setText(file.getFileNameWithoutExtension(), juce::dontSendNotification);
        }
        playlist->insertUniqueTrack(Track(file));
    }
}



//==============================================================================

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
};
