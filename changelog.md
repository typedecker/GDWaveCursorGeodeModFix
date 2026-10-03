## v1.3.6

- Fix build compatibility by removing unsupported Cocos2d global-z-order calls.
- Attach WaveCursor and its trails directly to the active scene at maximum local z-order so the cursor renders above Eclipse's Cocos UI.

# v1.3.5

- Fixed Eclipse/in-game overlay draw-order conflicts by assigning the Wave Cursor and its drawable children a high global z-order.
- Preserved v1.3.4 visibility behavior for normal gameplay, pause/completion screens, and in-level overlays.

# v1.3.4

- Fixed the WaveCursor appearing behind Eclipse and other in-level overlays by keeping the cursor at the highest overlay z-order.
- Preserved the v1.3.3 behavior: hidden during active gameplay, visible in pause/completion screens, and visible when an in-level menu requests the cursor.

# 1.3.3
- Show the Wave Cursor over external in-level overlays such as Eclipse while still hiding it during active gameplay.
- Keep the cursor visible in pause/completion screens and outside levels even when another mod requests the system cursor to hide.

# 1.3.2
- Hide the Wave Cursor while actively playing a level while keeping it visible in pause/completion screens and outside levels.

# 1.3.1
- Fixed a certain issue where a setting wouldn't apply

# 1.3.0

- The MacOS cursor now hides properly!
- The cursor should now properly enable and disable when other mods want it to
- Added soggy cat?

# 1.2.4  

- Fixed trail not hiding when setting was disabled

# 1.2.3

- Fix opacity crash?

# 1.2.2

- Fix race condition crash
- Fix certain scenarios where the cursor would not show up
- Fix trail bug, possibly.

# 1.2.1

- Fix on more games button causing a game crash.

# 1.2.0

- Trail should now appear more often
- Fix cursor showing up when its not supposed to
- Removed Alpha's UI dependency
- Internally changed how the platform cursor is hidden/shown to fix some issues  

# 1.1.3

- Added Cursor trails
- Fixed Changing to fullscreen breaking the cursor
- Fixed Show Cursor toggle not working  

# 1.0.1

- Fix lock cursor not working  

# 1.0.0  

- Added cursor  
- Added size setting  
