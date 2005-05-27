
                        --------------------------------
                        FMOD Sample Bank/Stream Compiler
                                      v1.14
                   Copyright Firelight Technologies 2002, 2003.
                        --------------------------------

Index.
------

1. Introduction.
2. Using it to generate 1 FSB file, or multiple single FSB files!
3. Hit 'Build' and notice the RAM usage bar graph.
4. Error dialog box
5. Using FSB files in your program.
    5.1 As a static sample bank.
    5.2 For streaming.
6. Support




1. Introduction.
----------------
This tool takes wav files, mp3 files, ogg files, aiff files etc, and converts them
to the .FSB format (FMOD Sample Bank).
It will compress them in the native format of the machine.  


2. Using it to generate 1 FSB file, or multiple single FSB files!
--------------------------------------------------------------
If you look to the top of the dialog, you will see a menu item called 'Build Mode'.
In this you can tell FSBank to write out 1 packed FSB file with all your sounds in it,
or alternatively write out 1 FSB for each sound you provide.  This is useful for streams.

- The default mode is 'Generate Single FSB File'

This means if you provide a source directory to compile from, it will compile every sound
in that directory structure recursively, into a single .FSB file.
This is mainly used for sample banks, or packing streams into a single file for indexed streaming.

- The second mode type is 'Generate Multiple FSB Files'

This mode means that for every sound in your specified source directory, it will generate
an equivalent FSB file in the specified destination directory.  This would only realistically be 
used for streams, as a sort of 'batch converter'.
It will build all files into the directory you specify by default.  If you want to recreate
the directory structure from the source directory, click 'Rebuild source directory tree to
destination'

- The third mode is 'Generate Single Interleaved FSB file'

This mode means it will take all input files, and interleave them into one FSB file.
For example, a normal stereo stream is interleaved as LRLRLRLR etc.
This is what it would look like if you provided 2 mono files or 1 stereo file as the source.
If you specify more, for example 4 mono files, and a stereo file, the resulting FSB looks like
this : 123456123456123456.  
What this means is multiple audio tracks can be played back as one stream, and seeking is eliminated.
Each channel within this multitrack stream can have its attributes modified (ie volume/pan) so you
can use this feature for interactive music, or mood based ambiences.  Many other uses could be found.




3. Hit 'Build' and notice the RAM usage bar graph.
-----------------------------------------------
Press the big 'Build' button and it will start compiling.

Notice the RAM usage bar graph below.  If you intend to load the FSB into ram for sample 
playback (not streamed), then you will have to make sure the graph does not go red.  This
means you will have overflowed sound ram and you will not fit all of your sounds in.

On PlayStation 2, SPU2 ram has 1,790kb available for sound if you are using full reverb.
If you uncheck 'Reverb Core0 enabled' or  'Reverb Core1 enabled' you gain 128k to give 1,918kb free.
If you uncheck 'Reverb Core1 enabled' and 'Reverb Core0 enabled' you gain 256k to give 2,046kb free.
Note disabling reverb has to be coupled in the code by using FSOUND_REVERB_FLAGS_CORE0 and/or
FSOUND_REVERB_FLAGS_CORE1 in the call to FSOUND_Init.

If you intend to use the FSB file(s) for streaming purposes, ignore the ram usage warnings as
streams can be of unlimited size.




4. Error dialog box
--------------------

When the build has completed, a list of warnings will appear.


'Memory for this platform has been exceeded'

If your usage is intended for static loading into memory for sample playback.  You have overrun
the hardware memory.
If you intend to use the file(s) for streaming then ignore this message.  On PlayStation 2, Streams 
use 4k of SPU2 memory if targeted for SPU2 (and not IOP).




5. Using FSB files in your program.
-----------------------------------

5.1 As a static sample bank.
----------------------------
To load a bank use FMUSIC_LoadSong.

To get access to the individual samples (FSOUND_SAMPLE) use FMUSIC_GetSample.

The C header contains #defines to reference the sound from your code with FMUSIC_GetSample.

You can even preview a .FSB file from your program with FMUSIC_PlaySong!  
It will simply play through all sounds in the bank sequentially and then stop.

5.2 For streaming.
------------------

To open an FSB file as a stream use FSOUND_Stream_OpenFile.

call FSOUND_Stream_Play to play the first stream in the FSB file.

call FSOUND_Stream_SetSubStream to make the stream seek to the relevant file.
use the #defines generated by FSBank in this function.
calling this will stop the stream if it is already playing.

FSOUND_Stream_SetSubStreamSentence can be used to 'stitch', or 'sentence' multiple substreams together.

6. Support
----------
Write to support@fmod.org for questions and help.

