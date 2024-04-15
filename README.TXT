
This is a Playstation 2 port of the original doom source code.

Features:

Cheats, Music, ADPCM sound effects, Memcard saving and loading, wad selecter, jumping.

The music is off. It runs on the TinySoundFont header library, which uses double precision for some calculations. 
Everything is converted to single precision which causes the pitch to be off. You can experiment with different
soundfonts, but it comes with gzdoom.sf2 which is ~3.25MB. I haven't experimented with anything bigger than that. Some slowdown occasionally
happens during a dma transfer to iop, but it runs fine otherwise.

I changed audsrv (included in the repo) to make it so the playback wasn't tied to the framerate. Now it runs on a triple-buffered setup where two buffers
are on the IOP for 5 seconds of music while another 5 seconds is stored on EE side until a IOP buffer is free. 

It runs on the cdvd currently and I only have tested it with pcsx2 emulator (and have never tested the drawing library on ps2 either). I think changes would have to be made to run 
with ps2link. 

This was more of an experiment for myself on how to port a game (albeit simply) but if you happen to play it, let me know
any feedback.

NOTE:

In order to compile, add your ps2sdk build path to the variable PS2SDKSRC

Thanks,

Drew Fletcher
