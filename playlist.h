#ifndef POTIFY_PLAYLIST_H
#define POTIFY_PLAYLIST_H

#include <QWidget>
#include <QDir>
#include <QStringList>
#include <QStringListModel>
#include <QLabel>
#include <QPushButton>
#include <QListView>

class Playlist : public QWidget {
Q_OBJECT

public:
    explicit Playlist(QWidget *parent = nullptr);

signals:

    void playlistUrl(QUrl file);

    void playlistName(QString name);

private slots:

    void openPlaylist();

    void listIndexChanged(const QModelIndex &index);

public slots:

    void requestNext();

    void requestPrevious();

    void requestShuffle();

private:
    int index;
    QDir directory;
    QStringList files;
    QStringListModel *files_model = nullptr;
    QLabel *playlist_label = nullptr;
    QPushButton *open_button = nullptr;
    QListView *playlist_view = nullptr;

    void setIndex();
};

#endif //POTIFY_PLAYLIST_H
