#include "audio.h"
#include "chip8.h"
#include "config.h"
#include "events.h"
#include "gui.h"
#include "rom.h"
#include "video.h"


int main(int argc, char* argv[])
{
    bool quit = false;

    configSetDefaults();
    configLoadFromFile();

    if (!videoInitialize())
    {
        if (!audioInitialize())
        {
            chip8ResetCpu();

            if (argc > 1)
            {
                romSetPath(argv[1]);
                romLoadFromPath();
            }
            else
            {
                romLoadDefault();
            }

            while (!quit)
            {
                quit |= eventsProcess();
                quit |= guiGetFlagQuit();

                if (chip8StepCpu(configGetCpuFreq() / 60))
                    guiShowUnknownOp();

                audioUpdate(chip8GetAudio());

                chip8UpdateTimers();

                videoRender();
            }

            configSaveToFile();

            audioFinalize();
        }

        videoFinalize();
    }

	return 0;
}
