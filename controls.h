#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QToolButton>
#include <QSlider>
#include <QMediaPlayer>
#include <QLabel>

class Controls : public QWidget {
    Q_OBJECT

public:
    explicit Controls(QWidget *parent = nullptr);
    bool playing;
    bool media;

signals:
    void play();
    void pause();
    void next();
    void previous();
    void changeVolume(float volume);

public slots:
    void playbackStateChanged(QMediaPlayer::PlaybackState newState);
    void mediaStateChanged(QMediaPlayer::MediaStatus newState);

private slots:
    void playClicked();
    void nextClicked();
    void previousClicked();
    void volumeChanged(int volume);

private:
    QToolButton *play_button = nullptr;
    QToolButton *next_button = nullptr;
    QToolButton *previous_button = nullptr;
    QSlider *volume_slider = nullptr;

};

#endif // CONTROLS_H
