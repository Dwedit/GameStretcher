# GameStretcher
GameStretcher is a program that allows you to run 2D Windows Games (games which do not use DirectX) with a stretchable window, and will use high-quality SuperXBR filtering to enlarge the graphics.

Normally, older games that use GDI will run in a fixed-size window, and are intended for smaller screen sizes such as 640x480 or 800x600, as opposed to modern common 1920x1080 resolution displays.  The window is usually not resizable.  This tool will allow you to resize and maximize the window.

Requires Windows XP or newer, and a Direct3D 9 capable video card.  Compatible with Wine.

# Instructions
Run Launcher.exe, and any other EXEs in the same directory will appear in a list, or click Browse to find the EXE yourself.

You can also create a shortcut to Launcher in the game directory.

Supports command line arguments, like "Launcher <program.exe> [args]" from command line or from a shortcut.

Add the --optimus switch before the exe name to force using the NVIDIA GPU on gaming laptops.

Use F11 to enter or exit Fullscreen mode. Do not try to use any fullscreen functionality that's built-in to the game itself, it won't work properly.

# Version History
1.0.2:
* Supports Direct3D 9 Games
* Supports DirectDraw Games
* Registry Virtualization for some situations where Windows itself messes up
* --optimus switch to force using NVIDIA GPU on gaming laptops

1.0.1:
* Fixed white screen before window is resized for the first time
* Fixed fullscreen mode showing the taskbar

1.0.0:
* Initial Release

# Screenshots
Space Cadet Pinball: Original and Upscaled

![Screenshot of Space Cadet Pinball, portion of screen](/readme-images/space_cadet_pinball_original_portion.png?raw=true "Screenshot of Space Cadet Pinball, portion of screen")

![Screenshot of Space Cadet Pinball, portion of screen (upscaled)](/readme-images/space_cadet_pinball_upscaled_portion.jpg?raw=true "Screenshot of Space Cadet Pinball, portion of screen (upscaled)")

Minesweeper: Original and Upscaled

![Screenshot of Minesweeper, portion of screen](/readme-images/minesweeper_screen_portion.png?raw=true "Screenshot of Minesweeper, portion of screen")

![Screenshot of Minesweeper, portion of screen (upscaled)](/readme-images/minesweeper_screen_portion_upscaled.png?raw=true "Screenshot of Minesweeper, portion of screen (upscaled)")

ROM Check Fail (game by Farbs, uses DirectDraw) Original and Upscaled

![Screenshot of ROM Check Fail, portion of screen](/readme-images/rom_check_fail_screen_portion.png?raw=true "Screenshot of ROM Check Fail, portion of screen")

![Screenshot of ROM Check Fail, portion of screen (upscaled)](/readme-images/rom_check_fail_screen_portion_upscaled.jpg?raw=true "Screenshot of ROM Check Fail, portion of screen (upscaled)")
