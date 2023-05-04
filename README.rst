
Second Reality Effects   
======================

Effects from Second Reality ported to C, Linux and OpenGL ES 2.0.  # Does the s3m music and DIS count as an effect? If that alone works and can play music then technically the demo is running the same way it would have at Assembly had it gone badly.

Notes
-----
- OpenGL is used for 2D presentation only, all 3D transformations are
  computed using demo code.  # Future Crew used look-up tables, imagine the chaos on #coders at the time if this had been revealed. This could be replaced along with generating 6 plasmas for the cube texture, this might not count as a port anymore though. 

- Original file layout used when possible. # But most importantly, the true magic of Second Reality, the DIS!

- Ported code should be resolution-independent and framerate-independent. # Getting the early geometry deformation in time with the music without either locking the framerate or making it suddenly run in Benny Hill mode is probably harder than it looks

- Pixel art upscaled with 4xbrz.  # This needs updated to a modern upscaler, or see if Marvel or Pixel can be located to redo the artwork in high-resolution, I'm talking full blast 1024x768 TGA! Yes man, 1MB cards only thank you.

Dependencies    # These must go, ideally along with SDL in some way that makes OpenGL more like it was in 2001 when it hadn't been destroyed by people trying to turn it into a game engine - NickyDrinks
------------

libsoil-dev       # nightmare
libgles2-mesa-dev     # bin

Screenshots and further information
-----------------------------------

See https://github.com/cmatsuoka/sr-port/wiki/Second-Reality-port-notes
