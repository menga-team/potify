#include "player.h"

#include <iostream>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QHBoxLayout>
#include <iomanip>

Player::Player(QWidget *parent) : QWidget(parent) {
    player = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.10);
    connect(player, &QMediaPlayer::errorOccurred, this, &Player::error);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &Player::playbackStateBridge);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &Player::mediaStateBridge);

    media_slider = new QSlider(Qt::Horizontal, this);
    media_label = new QLabel("--:--");
    connect(media_slider, &QSlider::sliderReleased, this, &Player::sliderReleased);

    connect(player, &QMediaPlayer::durationChanged, this, &Player::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &Player::positionChanged);

    QBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(media_slider);
    layout->addWidget(media_label);
    setLayout(layout);
}

// controls
void Player::play() {
    std::cout << "player: play" << std::endl;
    player->play();
}

void Player::pause() {
    std::cout << "player: pause" << std::endl;
    player->pause();
}

void Player::stop() {
    std::cout << "player: stop" << std::endl;
    player->stop();
}

void Player::setVolume(float volume) {
    std::cout << "player: setVolume: " << volume << std::endl;
    audioOutput->setVolume(volume);
}

// playlist
void Player::setSource(const QUrl &source) {
    std::cout << "player: setSource: " << source.toString().toStdString() << std::endl;
    player->setSource(source);
}

void Player::playbackStateBridge(QMediaPlayer::PlaybackState newState) {
    emit playbackStateChanged(newState);
}

void Player::mediaStateBridge(QMediaPlayer::MediaStatus newState) {
    emit mediaStateChanged(newState);
}

void Player::error(QMediaPlayer::Error error, const QString &errorString) {
    std::cout << "player: ERROR: " << errorString.toStdString() << std::endl;
}

// media slider
void Player::durationChanged(qint64 milliseconds) {
    std::cout << "player: duration changed to " << milliseconds / 1000 << std::endl;
    media_duration = milliseconds / 1000;
    media_slider->setMaximum((int) media_duration);
    calculateMediaLabel(media_slider->value());
}

void Player::positionChanged(qint64 milliseconds) {
    if (!media_slider->isSliderDown()) media_slider->setValue((int) milliseconds / 1000);
    calculateMediaLabel(milliseconds / 1000);
}

void Player::sliderReleased() {
    std::cout << "player: released; new value: " << media_slider->value() << " seconds" << std::endl;
    player->setPosition(media_slider->value() * 1000);
    calculateMediaLabel(media_slider->value());
}

void Player::calculateMediaLabel(qint64 milliseconds) {
    std::cout << "player: calculateMediaLabel " << std::endl;

    std::stringstream sCurrentTimeBuffer;
    std::string currentTime;
    std::stringstream sMaxTimeBuffer;
    std::string totalTime;

    // format ms to correct time format
    auto s1 = static_cast<qint64>(milliseconds);
    qint64 h1 = s1 / (60 * 60);
    s1 -= h1 * (60 * 60);
    qint64 m1 = s1 / (60);
    s1 -= m1 * (60);

    // format max time to correct time format
    auto s2 = static_cast<qint64>(media_duration);
    qint64 h2 = s2 / (60 * 60);
    s2 -= h2 * (60 * 60);
    qint64 m2 = s2 / (60);
    s2 -= m2 * (60);

// With hours
//    sCurrentTimeBuffer << std::setfill('0') << std::setw(2) << h1 << ':' << std::setw(2) << m1 << ':' << std::setw(2) << s1 << std::endl;
//    currentTime = sCurrentTimeBuffer.str();
//
//    sMaxTimeBuffer << std::setfill('0') << std::setw(2) << h2 << ':' << std::setw(2) << m2 << ':' << std::setw(2) << s2 << std::endl;
//    totalTime = sMaxTimeBuffer.str();

    // Without hours
    sCurrentTimeBuffer << std::setfill('0') << std::setw(2) << m1 << ':' << std::setw(2) << s1 << std::endl;
    currentTime = sCurrentTimeBuffer.str();

    sMaxTimeBuffer << std::setfill('0') << std::setw(2) << m2 << ':' << std::setw(2) << s2 << std::endl;
    totalTime = sMaxTimeBuffer.str();

    updateMediaLabel(currentTime, totalTime);
}

void Player::updateMediaLabel(const std::string &currentTime, const std::string &totalTime) {
    std::cout << "player: updateMediaLabel; current  " << currentTime;
    std::cout << "player: updateMediaLabel; total " << totalTime;
    media_label->setText(QString::fromStdString(currentTime + " / " + totalTime));
}