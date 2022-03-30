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
    media_label = new QLabel("00:00:00");
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
    media_duration = milliseconds;
    media_slider->setMaximum(media_duration / 1000);
}

void Player::positionChanged(qint64 milliseconds) {
    if (!media_slider->isSliderDown()) media_slider->setValue(milliseconds / 1000);
    updateMediaLabel(milliseconds / 1000);
}

void Player::sliderReleased() {
    std::cout << "player: released; new value: " << media_slider->value() << " seconds" << std::endl;
    player->setPosition(media_slider->value() * 1000);
    updateMediaLabel(media_slider->value());
}

void Player::updateMediaLabel(qint64 position) {

    // format position to correct time format
    auto s = static_cast<qint64>(position);
    qint64 h = s / (60 * 60);
    s -= h * (60 * 60);

    qint64 m = s / (60);
    s -= m * (60);

    // write into stringstream, then convert to string --> QString
    std::stringstream buffer;
    std::string buffer2;

    buffer << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2) << m << ':' << std::setw(2) << s << std::endl;
    buffer2 = buffer.str();
    media_label->setText(QString::fromStdString(buffer2));
}