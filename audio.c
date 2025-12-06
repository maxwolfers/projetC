#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "audio.h"


void lancerMusique(char *cheminFichier) {
    PlaySound(TEXT(cheminFichier), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
}

void arreterMusique() {
    PlaySound(NULL, 0, 0);
}