/* mbed  Video Player 
 */
#ifndef MEDIO_PLAYER_H
#define MEDIO_PLAYER_H

#include "mbed.h"
#include "AudioPlayer.h"
#include "VideoPlayer.h"
#include "FATFileSystem.h"
#include "ILI9341V.h"


class MediaPlayer : private NonCopyable<MediaPlayer> {
public:

    MediaPlayer(ILI9341V *lcd, PinName audio_pwm_out);

    bool play(FATFileSystem* fs, const char* file_name, uint32_t fps);

    void stop();

    ~MediaPlayer();

private:
    // audio
    void _play_audio();
    PwmOut _pwm_out;
    AudioPlayer _audio_player;
    Thread _audio_thread;
    File _audio_file;

    //video
    void _play_video();    
    VideoPlayer _video_player;
    uint32_t _fps;    
    Thread _video_thread;
    File _video_file;

};

#endif
