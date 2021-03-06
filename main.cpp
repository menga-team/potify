#include <QApplication>
#include <QVBoxLayout>
#include <QAudioOutput>
#include <QFileDialog>
#include <QStyle>
#include <QSlider>

#include "controls.h"
#include "playlist.h"
#include "player.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));

    // main widget
    QWidget window = QWidget();
    QVBoxLayout mainlayout = QVBoxLayout();
    window.setWindowTitle("potify");
    window.resize(480, 320);

    // playlist
    Playlist playlist = Playlist();
    mainlayout.addWidget(&playlist);

    // player
    Player player = Player();
    mainlayout.addWidget(&player);

    // controls
    Controls controls = Controls();
    mainlayout.addWidget(&controls);

    // media player state changed -> update controls
    QObject::connect(&player, &Player::playbackStateChanged, &controls, &Controls::playbackStateChanged);
    QObject::connect(&player, &Player::mediaStateChanged, &controls, &Controls::mediaStateChanged);

    // play/pause buttons & volume slider & stop instruction -> player
    QObject::connect(&controls, &Controls::play, &player, &Player::play);
    QObject::connect(&controls, &Controls::pause, &player, &Player::pause);
    QObject::connect(&controls, &Controls::stop, &player, &Player::stop);
    QObject::connect(&controls, &Controls::changeVolume, &player, &Player::setVolume);

    // prev/next/shuffle instructions -> playlist -> player
    QObject::connect(&controls, &Controls::next, &playlist, &Playlist::requestNext);
    QObject::connect(&controls, &Controls::previous, &playlist, &Playlist::requestPrevious);
    QObject::connect(&playlist, &Playlist::playlistUrl, &player, &Player::setSource);
    QObject::connect(&controls, &Controls::shuffle, &playlist, &Playlist::requestShuffle);

    // list clicked -> controls -> playlist -> player
    QObject::connect(&playlist, &Playlist::listClicked, &controls, &Controls::listClicked);
    QObject::connect(&controls, &Controls::specific, &playlist, &Playlist::requestSpecific);

    window.setLayout(&mainlayout);
    window.show();

    return QApplication::exec();
}