#include "controls.h"

#include <iostream>
#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QAudio>
#include <QMediaPlayer>

Controls::Controls(QWidget *parent) : QWidget(parent) {
    playing = false;
    media = false;
    shuffle_bool = false;
    instruction = 0;
    specific_index = 0;

    play_button = new QToolButton(this);
    play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(play_button, &QAbstractButton::clicked, this, &Controls::playClicked);

    next_button = new QToolButton(this);
    next_button->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(next_button, &QAbstractButton::clicked, this, &Controls::nextClicked);

    previous_button = new QToolButton(this);
    previous_button->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(previous_button, &QAbstractButton::clicked, this, &Controls::previousClicked);

    volume_slider = new QSlider(Qt::Horizontal, this);
    volume_slider->setRange(0, 100);
    volume_slider->setValue(10);
    connect(volume_slider, &QSlider::valueChanged, this, &Controls::volumeChanged);

    shuffle_button = new QToolButton(this);
    shuffle_button->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(shuffle_button, &QAbstractButton::clicked, this, &Controls::shuffleClicked);

    QBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(previous_button);
    layout->addWidget(play_button);
    layout->addWidget(next_button);
    layout->addWidget(volume_slider);
    layout->addWidget(shuffle_button);
    setLayout(layout);
}

// why this? by stopping the player and then setting a new song we avoid weird bugs.
void Controls::playbackStateChanged(QMediaPlayer::PlaybackState newState) {
    // stopped
    if (newState == 0) {
        std::cout << "controls: playerStateChanged: stopped --> instruction " << instruction << std::endl;
        // 0 -> the song is finished -> autoplay && 2 -> next button -> next song
        if (instruction == 0 || instruction == 2) { if (shuffle_bool) emit shuffle(); else emit next(); }
            // 1 -> previous button -> previous song
        else if (instruction == 1) emit previous();
        else if (instruction == 3) emit specific(specific_index);
        // reset instruction
        instruction = 0;
        // transfer play/pause state from previous song
        if (playing) emit play();
        else emit pause();
    }
        // playing -> set icon
    else if (newState == 1) {
        std::cout << "controls: playerStateChanged: playing" << std::endl;
        play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
        // paused -> set icon
    else {
        std::cout << "controls: playerStateChanged: paused" << std::endl;
        play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void Controls::mediaStateChanged(QMediaPlayer::MediaStatus newState) {
    // media has been loaded
    if (newState == 2) media = true;
        // no media
    else if (newState == 0) media = false;
}

void Controls::playClicked() {
    if (!media) return;
    // not playing -> play
    if (!playing) {
        std::cout << "controls: play" << std::endl;
        emit play();
    }
        // playing -> pause
    else {
        std::cout << "controls: pause" << std::endl;
        emit pause();
    }
    playing = !playing;
}

void Controls::nextClicked() {
    if (!media) return;
    std::cout << "controls: next" << std::endl;
    instruction = 2;
    emit stop();
}

void Controls::previousClicked() {
    if (!media) return;
    std::cout << "controls: previous" << std::endl;
    instruction = 1;
    emit stop();
}

void Controls::volumeChanged(int volume) {
    std::cout << "controls: volume: " << volume << std::endl;
    emit changeVolume((float) volume / 100);
}

void Controls::shuffleClicked() {
    if (shuffle_bool) shuffle_button->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    else shuffle_button->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    shuffle_bool = !shuffle_bool;
    std::cout << "controls: shuffle: " << shuffle_bool << std::endl;
}

void Controls::listClicked(int index) {
    specific_index = index;
    instruction = 3;
    emit stop();
}