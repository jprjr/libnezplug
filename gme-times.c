#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gme/gme.h>

int main(int argc, char *argv[]) {
    char *filename = NULL;
    gme_t *Emu = NULL;
    gme_err_t err = NULL;
    gme_info_t *info = NULL;
    char *p = NULL;
    uint32_t m3uSize = 0;
    uint32_t i = 0;

    if(argc < 2) {
        return 1;
    }


    err = gme_open_file(argv[1],&Emu, gme_info_only);
    if(Emu == NULL) return 1;
    if(err != NULL) {
        printf("Error: %s\n",err);
        return 1;
    }

    p = strrchr(argv[1],'.');
    if(p == NULL) return 1;

    *p = 0;

    m3uSize = snprintf(NULL,0,"%s.m3u",argv[1]);
    filename = (char *)malloc(m3uSize + 1);
    if(filename == NULL) return 1;

    snprintf(filename,m3uSize+1,"%s.m3u",argv[1]);
    err = gme_load_m3u(Emu,filename);
    if(err != NULL) {
        printf("Error loading m3u: %s\n",err);
        return 1;
    }

    printf("Track count: %d\n",gme_track_count(Emu));
    for(i=0;i<gme_track_count(Emu);i++) {
        gme_track_info(Emu,&info,i);
        printf("Track %02d: \n"
        "\ttitle: %s\n"
        "\tlength: %d\n"
        "\tintro_length: %d\n"
        "\tloop_length: %d\n"
        "\tplay_length: %d\n"
        "\tfade_length: %d\n",
            i,
            info->song,
            info->length,
            info->intro_length,
            info->loop_length,
            info->play_length,
            info->fade_length);


        gme_free_info(info);
    }

    return 0;
}
