#include "music.h"

Music::Music()
{
    player = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/media/background.aac"));
    player->setPlaylist(playlist);
    player->play();
}
