#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../src/include/nezplug/nezplug.h"

static uint8_t *slurp(char *filename, uint32_t *size);

static uint8_t *slurp(char *filename, uint32_t *size) {
    uint8_t *buf;
    FILE *f = fopen(filename,"rb");
    if(f == NULL) {
        fprintf(stderr,"Error opening %s: %s\n",
          filename,
          strerror(errno));
        return NULL;
    }
    fseek(f,0,SEEK_END);
    *size = ftell(f);
    fseek(f,0,SEEK_SET);

    buf = (uint8_t *)malloc(*size);
    if(buf == NULL) {
        fprintf(stderr,"out of memory\n");
        return NULL;
    }
    if(fread(buf,1,*size,f) != *size) {
        fprintf(stderr,"error reading file\n");
        free(buf);
        return NULL;
    }
    return buf;
}

int main(int argc, char *argv[]) {
    NEZ_PLAY *player;
    uint8_t *data;
    uint32_t size;
    unsigned int i;
    char *filename;
    int16_t *buffer;
    uint32_t samples;
    const int channels = 2;

    if(argc < 2) {
        fprintf(stderr,"Usage: bench /path/to/file\n");
        fprintf(stderr,"  decodes all tracks to PCM\n");
        return 1;
    }

    filename = NULL;

    buffer = (int16_t *)malloc(sizeof(int16_t) * channels * 4096);
    if(buffer == NULL) abort();

    data = slurp(argv[1],&size);
    if(data == NULL) return 1;

    player = NEZNew();
    if(player == NULL) {
        free(data);
        return 1;
    }

    if(NEZLoad(player,data,size)) {
        NEZDelete(player);
        free(data);
        return 1;
    }
    free(data);

    NEZSetFrequency(player,48000);
    NEZSetChannel(player,channels);

    filename = NULL;
    for(i=NEZGetSongStart(player);i<=NEZGetSongMax(player);i++) {
        NEZSetSongNo(player,i);
        NEZReset(player);

        /* convert 2 minutes of audio */
        samples = 0;
        while(samples < 48000 * 120) {
            NEZRender(player,buffer,4096);
            samples += 4096;
        }

    }


    NEZDelete(player);
    free(filename);
    free(buffer);
    return 0;
}





