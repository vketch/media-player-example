#include <chrono>
#include <string.h>
#include <string>
#include "MediaPlayer.h"
#include "VideoPlayer.h"


#define PWM_OUT_PERIOD_US    32 //in microseconds
#define MAX_SIZE_FILE_NAME   128 


using namespace std;
using namespace std::chrono;


char* get_audio_name(const char *origin_name,  char *result_name ){
    strlcpy(result_name, origin_name, MAX_SIZE_FILE_NAME);                
    char *pdot = strstr(result_name, ".");

    if( pdot != NULL ){
        strlcpy(pdot, ".wav", 5);
        return result_name;
    }
    else{
        return NULL;
    }
}

MediaPlayer::MediaPlayer(ILI9341V *lcd, PinName audio_pwm_out):
   _pwm_out(audio_pwm_out), _audio_player(&_pwm_out), _video_player(lcd)
{    
    _pwm_out.period_us(PWM_OUT_PERIOD_US);
    _pwm_out.write(0.5);
}

MediaPlayer::~MediaPlayer(){};

void MediaPlayer::_play_audio(){
    debug("start play audio\n");
    if (_audio_player.play(&_audio_file) == false){
        printf("Error on playing audio!\n");  
    }
    debug("finish play audio\n");
    _audio_file.close();
}

void MediaPlayer::_play_video(){

    debug("start play video\n");
    if (_video_player.play(_video_file, _fps) == false){
        printf("Error on playing video!\n");  
    }
    debug("finish play video\n");
    _video_file.close();
}


bool MediaPlayer::play(FATFileSystem* fs, const char* file_name, uint32_t fps){

    int res = _video_file.open(fs, file_name);
    if (res != 0) {
        error("Could not open '%s, errocode=%d'\n", file_name, res);
        return false;
    }


    char audio_file_name[MAX_SIZE_FILE_NAME];
    get_audio_name(file_name, audio_file_name);

    res = _audio_file.open(fs, audio_file_name);
    if (res != 0) {
        error("Could not open '%s, errocode=%d'\n", audio_file_name, res);
        return false;
    }

    _fps = fps;
    _video_thread.start( callback(this, &MediaPlayer::_play_video) );

    _audio_thread.start( callback(this, &MediaPlayer::_play_audio) );    

    return true;
}

void MediaPlayer::stop(){
    _audio_player.stop();
    _video_player.stop();
}
