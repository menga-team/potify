#include "playlist.h"

#include <iostream>
#include <QBoxLayout>
#include <QSlider>
#include <QAudio>
#include <QPushButton>
#include <QFileDialog>
#include <QStringListModel>
#include <random>

Playlist::Playlist(QWidget *parent) : QWidget(parent) {
    index = 0;

    open_button = new QPushButton(this);
    open_button->setText("Open Playlist");
    connect(open_button, &QAbstractButton::clicked, this, &Playlist::openPlaylist);

    playlist_label = new QLabel(this);
    playlist_label->setText("No Playlist Selected");
    connect(this, &Playlist::playlistName, playlist_label, &QLabel::setText);

    playlist_view = new QListView(this);
    playlist_view->setEditTriggers(QListView::NoEditTriggers);
    files_model = new QStringListModel(this);
    playlist_view->setModel(files_model);

    connect(playlist_view, &QListView::clicked, this, &Playlist::listIndexChanged);

    //TODO connect arrow key selection with signal/slot

    QBoxLayout * layout = new QVBoxLayout;
    QBoxLayout * topbar = new QHBoxLayout;
    topbar->addWidget(playlist_label);
    topbar->addWidget(open_button);
    layout->addLayout(topbar);
    layout->addWidget(playlist_view);
    setLayout(layout);
}

void Playlist::requestNext() {
    std::cout << "playlist: requestNext" << std::endl;
    index++;
    if (index >= files.length()) index = 0;
    emit playlistUrl(QUrl::fromLocalFile(directory.absoluteFilePath(files.value(index))));
    setIndex();
    std::cout << "playlist: now playing index " << index << ": " << files.value(index).toStdString() << std::endl;
}

void Playlist::requestPrevious() {
    std::cout << "playlist: requestPrevious" << std::endl;
    index--;
    if (index < 0) index = (int) files.length() - 1;
    emit playlistUrl(QUrl::fromLocalFile(directory.absoluteFilePath(files.value(index))));
    setIndex();
    std::cout << "playlist: now playing index " << index << ": " << files.value(index).toStdString() << std::endl;
}

void Playlist::openPlaylist() {
    std::cout << "playlist: openPlaylist" << std::endl;
    QString new_directory = QFileDialog::getExistingDirectory(this, "Select Playlist Directory");
    if (new_directory.isNull()) return;
    else directory = new_directory;
    files = directory.entryList(QStringList() << "*.mp3" << "*.MP3" << "*.wav" << "*.WAV" << "*.m4a" << ".M4A" << "*.aac" << "* .AAC" << "*.flac" << "*.FLAC" << "*.webm", QDir::Files);
            foreach(QString filename, files) { std::cout << directory.absoluteFilePath(filename).toStdString() << std::endl; }
    files_model->setStringList(files);
    playlist_label->setText(directory.dirName());
    emit playlistUrl(QUrl::fromLocalFile(directory.absoluteFilePath(files.value(index))));
    setIndex();
}

void Playlist::listIndexChanged(const QModelIndex &changed_index) {
    index = changed_index.row();
    emit listClicked(index);
}

void Playlist::setIndex() {
    QModelIndex _index = files_model->index(index, 0, QModelIndex());
    playlist_view->setCurrentIndex(_index);
}

void Playlist::requestShuffle() {
    std::cout << "playlist: requestShuffle" << std::endl;

    std::random_device random;
    std::mt19937 mt(random());
    std::uniform_int_distribution<int> dist(0.0, (int) files.length());

    std::cout << dist(mt) << std::endl;
    index = dist(mt);

    if (index >= files.length()) index = 0;
    emit playlistUrl(QUrl::fromLocalFile(directory.absoluteFilePath(files.value(index))));
    setIndex();

    std::cout << "playlist: now playing index " << index << ": " << files.value(index).toStdString() << std::endl;
}

void Playlist::requestSpecific(int specific_index) {
    std::cout << "playlist: requestSpecific" << std::endl;
    index = specific_index;
    emit playlistUrl(QUrl::fromLocalFile(directory.absoluteFilePath(files.value(index))));
    setIndex();

    std::cout << "playlist: now playing index " << index << ": " << files.value(index).toStdString() << std::endl;
}