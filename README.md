# gameboy-glitch-shooter
Bullets don't stop writing to memory when they leave the screen. I'm sure everything will be fine.

# Compiling
You'll need the gbdk. I'm on MacOS and I had to find a prebuilt version to get one that even remotly works. I don't have it in me to explain this right now but @ me on twitter if you want the build. If you're on windows I can't help you.

In the makefile, change CC to the path to your gbdk compiler, and EMU to wherever your emulator is (if it doesn't use wine, you'll have to remove that from the run section of the makefile.

Run `make` to build and run. `make build` to just build the rom.
