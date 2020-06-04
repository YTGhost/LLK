#ifndef MUSIC_H
#define MUSIC_H
#include <QMediaPlayer>
#include <QMediaPlaylist>


class Music
{
public:
    Music();
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
};

#endif // MUSIC_H
