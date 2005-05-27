1.) Building the project

a) You need to download the DirectX 9 SDK, e.g. from 
http://www.microsoft.com/downloads/details.aspx?FamilyID=1d97f320-9dfd-4e7a-b947-3a037ccf84af&displaylang=en

b) Replace all "G:\DXSDK\" strings by your DX9 SDK install path in the include and library project settings.

2.) MIDI interface

a) Install your MIDI HUB
b) Find out the correct MIDI In port number
   - Modify the SoText3 node so that it looks like to following:
        SoText3 {
            string = USE PIANO.inputPortList
        }        
   - Run the pianotutor_keyboard.bat file
   - Check the graphical list and remember the number of your desired input port 
     (e.g. in the case of the MT4 MIDI HUB it should be "MT 4 In 02", which is #1 (the 2nd item in the list but we count from 0))
   - Set this number (in the example case 1) in the inputPortPreferred field of the Piano nodekit

c) Find out the correct MIDI Out port number
   - Modify the SoText3 node so that it looks like to following:
        SoText3 {
            string = USE PIANO.outputPortList
        }        
   - Run the pianotutor_keyboard.bat file
   - Check the graphical list and remember the number of your desired output port 
     (e.g. in the case of the MT4 MIDI HUB it should be "MT 4 Out 01", which is #5 (the 6th item in the list but we count from 0))
   - Set this number (in the example case 5) in the outputPortPreferred field of the Piano nodekit

c) For the desktop-based setup run pianotutor_desktop.bat

d) The order of the buttons of the PIP is the following
   - play MIDI file
   - pause/resume MIDI file
   - stop MIDI file
   - play MIDI chord on keyboard (with MIDI out)
