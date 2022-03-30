#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QLabel>

class Player : public QWidget {
Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);

signals:
    void playbackStateChanged(QMediaPlayer::PlaybackState newState);
    void mediaStateChanged(QMediaPlayer::MediaStatus newState);

private slots:
    void playbackStateBridge(QMediaPlayer::PlaybackState newState);
    void mediaStateBridge(QMediaPlayer::MediaStatus newState);
    void error(QMediaPlayer::Error error, const QString &errorString);
    void durationChanged(qint64 milliseconds);
    void positionChanged(qint64 milliseconds);
    void sliderReleased();

public slots:
    void play();
    void pause();
    void stop();
    void setVolume(float volume);
    void setSource(const QUrl &source);

private:
    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutput = nullptr;
    qint64 media_duration{};
    QSlider *media_slider = nullptr;
    QLabel *media_label = nullptr;

    void updateMediaLabel(qint64 position);

};

#endif // PLAYER_H