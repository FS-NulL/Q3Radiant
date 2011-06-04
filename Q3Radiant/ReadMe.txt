Build 136-137 Changes
---------------------
Added
- qer_editorimage flag to shader parsing
- md3 support for entities 
- md3 support for misc_model <- this works but it is a cluster fuck of code
| the code tries to pick up the same name for a skin (in the same path)
| grid snapping is not happening, i need to fix this
| some models (like pentagram1.md3) appear to be OFF as far as their constructed origin
| others (all others that i tried) are correct
| Free rotation does not work for these but the entity rotation stuff works fine
- Model... button in entity window for assigning model=value to current entity
- Sound... button in entity window for assigning sound=value to current entity


Build 134 Changes
-----------------
Added
- qer_nocarve flag
- qer_trans flag use 'qer_trans sometransvalue' marks a shader as transparent
- patches draw transparent based on shader information
- default fog setting changed from 0.15 to 0.35
- Plugin menu added (non functional yet)
- Fonts default to standard windows gui font for improved readibility
- Textures associated with a shader have [] around the name
- Shift-clicking a texture associated with a shader brings up the shader in notepad

Changed
-Max brush size bumped to 8192 (Brian Hook for testing), did not test with build tools

Fixed
-Entity dialog lets you type in more than visible space for key/val pairs
-Transparent bug when transparent brushes painted in front (visually) of a curve


Build 127 Changes
-----------------
Added
-Ctrl+Shift+P, this cycles the cap texturing axis on the selected cap. Basically, until
I figure out what the heck is wrong with the auto stuff, use this to correctly texture
a cap that is fubar'd.
-CAP texture button in surface dialog. This does the same as above but uses the default
axis finder (which has the bug).
-Matrox transposition

Build 126 Changes
-----------------
Removed
-Test code for thickening. It was hooked into overlay clear... oops.

Changed
-csg ignores patch symbiot brushes


Build 125 Changes
-----------------
Fixed
-Shift vertex (row/col) selection for patches.

Build 124 Changes
-----------------
Changed
- Temporarily reverted back to patchDef2 .map file format.

Build 123 Changes
-----------------

Changed
-.map patch format, added 'type' field. not used anywhere except the editor (at least for now)
- Patch vertex manipulation. The editor tries to pick all existing points from other patches as well
|You will want to do area selection (in an XY-type view tp select thick patch points if you want to drag
|everything related to the thick patch

Added
- ESC will deselect any patch vertex points selected instead of going completely out of vertex mode
|This only happens with patch vertex selection
- Thick patches, Curve | Thicken... or press Ctrl + T. The thickening happens away from the texture
|side of the patch. So if you want something to thicken inwards, make sure the texture is facing out.


Build 122 Changes
-----------------

Added
-Texture Flush (forces a new map, then a total texture flush)
|One small nit on this is that if you create a brush before loading a new set of textures it
|will use one of the recently loaded textures instead of the default no texture (blue and black checkerboard)
-Patch inverting. 
|Ctrl + I invertest the patch, 
|Shift + I inverts texture in X, 
|Ctrl + Shift + I inverts texture in Y
-Overlay function for patches. Turn on|off using menus or press 'Y' to make a patch an overlay and 
|ctrl+shift+Y to clear all overlays

Fixed
-Inverted beveles cap and texture properly


Build 121 Changes
-----------------
Added
-Patch texture inverting. Ctrl + Shift + I. I think this does what you wanted.
-'S'urface dialog grabs texture name from a single selected patch

Better (a little)
-Capping bevels makes the cap the right shape but the 3rd point is not on the right plane. 
In most cases you can move it and snap it to the right place. Texturing is not all fucked 
up anymore either. Once I hear back from John on my math stupidity, I will finish fixing it.
These statements are probably only true for non-bent bevels.. bending will probably screw
stuff up again.

Build 120 Changes
-----------------
Added
-Fine tune texturing for Patches. Using the surface dialog, you can type in very small 
(3 decimal places) numbers for fine tuning on both scale and shift. Currently you can
only fine tune with the Surface dialog, I am in the process of adding a fine tune settings
dialog for use with the keypad keys.

Build 119 Changes
-----------------
Added
-TAB cycles through any entity grouped brush
-Texture window can now be scaled by 10, 25, 50, 100, 200%
-Texture window now has a optional scroll bar, turn it on in prefs

-Texture path linkage
Put a "textureinfo.ini" file in a texture directory and add the following

[include]
Path0 = e:\games\quake2\plop\textures\24 bit\

You can have Path0 - Path9 as inclusive directories. The linked pathes will not
load and process link information to keep someone from linking two paths together
and causing an endless loop


Build 116 Changes
-----------------
Added
-Ctrl-P Toggle show patches

Fixed
-Gamma correction

Removed
-Model preview load code which was probably causing brandon's crash


Build 115 Changes
Tweaked
-Cap texturing
-Ctrl S was already bound to File | Save
-Ctrl C is bound to standard windows Copy

Added
-Ctrl-S Save file

Build 114 Changes
-----------------
Tweaked
-Cap texturing

Added
-ALT + Shift click will cycle through "selectable" brushes along the 
line of site from the mouse click. Works in all views. This is cool.
-Square cylinders

Fixed
-Show curves works properly

Pending
-Shifting textures on patches should honor scale (cannot dup)
-Disperse columns crashes (cannot dup)
-Capping bevels is messed up
-cap texturing may still be fubar'd
-Thick pipe primitive
-Scale origin + point scale similar to rotate. Will allow fluted stuff etc.. 

Build 113 Changes
-----------------
Added
-Capping (with dialog for Bevels and Endcaps)

Build 112 Changes
-----------------
Fixed
-Yet another surface dialog glitch
-Area selection tool paints in all xy oriented views

Build 111 Changes
-----------------
Fixed
-Surface dialog face selection bug

Build 110 Changes
-----------------
Added
-'B' - toggles Bend mode

Fixed
-Naturalized texturing on patches
-Bend whackiness on intial drag during start of a bend
-Setting bend rotation axis in XY Top view


Build 109 Changes
-----------------
Added
-Ctrl 'I' - inverts a curve
-Ctrl 'Numpad +' adds a row
-Ctrl 'Numpad -' removes a row
-Ctrl+Shift 'Numpad +' adds a column
-Ctrl+Shift 'Numpad -' removes a column
-Ctrl 'E' - redisperses rows
-Ctrl+Shift 'E' - redisperses columns
-Ctrl 'G' snaps patches to the nearest grid
-Surface control dialog now allows values to be typed in and applied to patches. 
 This is working except for shifts which still go in .9 and 1.1 amounts. 

Changed
-Regioning no longer puts a top and bottom on the region

Build 108 Changes
-----------------
Added
-Area selection. Works in 3 xy views but only draws in the xy view. I will look into this asap.

Changed
-Selection point size for vertex mode with patches. i.e. The handles for dragging patch points around are bigger.

Build 107 Changes
-----------------
Changed
-Inverted curve options removed from menu
-Bevels and endcaps are contsructed properly 
-Endcaps will not work for bevels and endcaps

Build 106 Changes
-----------------
Fixed
-shift middle clicking a light now adds the correct average color based on .TGA files
-display updates properly when using TAB to go between func_group'd brushes
-Patch texture rotation is much better. Still not completely perfect but should be usable.

Build 105 Changes
-----------------

Added
-TAB cycles through func_grouped brushes
-textures with a shader have a [] wrapped around the name in the texture window
-shift + left clicking on a shader texture in the texture window will bring up notepad with the shader file