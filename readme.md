# Jagged Alliance 2 Vanilla

This project is for maintaining Jagged Alliance 2 source codes
in buildable state with minimal modifications.

Goals:
- keep the sources buildable with modern versions of Visual Studio
- keep the game playable reasonably well on modern versions of Windows

These sources are very easy to change, but it's even easier to introduce
bugs by doing it.

Hence, the rules:
- keep changes to the minimum
- fix only critial bugs and issues
- don't change sources for the sake of it
- don't introduce any new features

This project is only for maintaining the vanilla game.

## How to build

- open the solution in Visual Studio 2022
- choose Release configuration
- choose Build Solution

## How to play the game

- install the original version of the game (from the original game CDs, Steam, gog.com, etc.)
- copy the builded exe file to the game directory alongside the original ja2.exe
- launch the builded exe file

The game was tested on Windows 7 and 10.  Playback of video clips is disabled
on Windows 10 because it hangs the game.

## License

License [SFI-SCLA](SFI-SCLA.txt).
