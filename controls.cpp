#include "controls.h"

#include <iostream>
#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QAudio>
#include <QMediaPlayer>

Controls::Controls(QWidget *parent): QWidget(parent) {
    playing = false;
    media - false;
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
    volume_slider->setValue(75);
    connect(volume_slider, &QSlider::valueChanged, this, &Controls::volumeChanged);

    QBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(previous_button);
    layout->addWidget(play_button);
    layout->addWidget(next_button);
    layout->addWidget(volume_slider);
    setLayout(layout);
}

void Controls::playbackStateChanged(QMediaPlayer::PlaybackState newState) {
    // stopped -> a new file got loaded -> set previous state
    if (newState == 0 ) {
        if (playing) emit play();
        else emit pause();
    }
    // playing -> set icon
    else if (newState == 1) {
        play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    // paused -> set icon
    else {
        play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
void Controls::mediaStateChanged(QMediaPlayer::MediaStatus newState) {
    if (newState == 2) media = true; // media has been loaded
    else if (newState == 0) media = false; // no media
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
    emit next();
}
void Controls::previousClicked() {
    if (!media) return;
    std::cout << "controls: previous" << std::endl;
    emit previous();
}
void Controls::volumeChanged(int volume) {
    std::cout << "controls: volume: " << volume << std::endl;
    emit changeVolume((float)volume / 100);
}