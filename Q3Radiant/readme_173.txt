Build 173

Changed
- Copy/Paste now uses the Win32 mechanism. This greatly speeds up editor to editor copy/paste.


Build 172

Added
- Hide and Unhide brush(s). Hide ( View Menu and 'H' ), hides the selected brushes. Shift-H shows anything previously hidden.
- Fit functionality in the Surface dialog no longer fubars the FIT exiting the dialog via OK or APPLY.
- Copy/Paste now functions between two open copies of the editor
- Textures | Show In Use now pays attention to Patches.
- The editor will now properly find a default project based on the assumption it lives in the \quake3\q3radiant\ path


In Progress
- Pref Dialog now has functioning Q3 and Q2 options.
- ASE Model load. Should be bug free in another evening.
- ZIP support
- Model Plugins for supporting various model types ( MD4, TIKI, MEN, ASE ) This is essentially moving all of the model preview, interaction, and animation to a plugin to clean the code up.

Deferred
- The new file format changes and new texturing is finished but based on the probable ship date(s), I dont feel comfortable mucking with the map format at this time. This code will be used in the new editor or can be rolled out once Q3 is gone.




Build 169-171
Added
- Control + Shift + X toggles crosshairs
- Added Fit functions to surface dialog. These are more test stuff than anything as the new texturing capabilities coming with the new file format will allow much more versatile texturing.

In Progress 
-New file format changes, true ST texturing for brushes, automatic conversion. This will happen soon.


Build 168
Added
-Control + Texture Click to apply texture applies with proper scale on angled faces.

Build 167
---------
Fixed
-Crash when edge dragging brushes with more than 1K points (total)


Build 166 changes
-----------------
Added
-Shift-C cap current patch
-Shift-A will now highlight all patches and caps based on 1. the currently selected face or 2. the current texture if there is no brush or face selected. If a brush is selected it still searches and highlights based on entity class.

Build 165 changes
-----------------
Added
-Texture lock status information, on the right edge status pane, L:move,rotate status. so L:MR means both move and rotate are selected, L: R means just rotate, etc.. Shift-R and Shift-T are hotkeys (always have been) for the locking. Help|Command List has a list of all bound keys.

Changed
-Curve primitives moved from a submenu back to the root.

Fixed
-Crash when pressing 'v' with large number of brushes selected.


Build 164 changes
-----------------
Added
-Brush | Primitive | Sphere - With grip snapping on, this produces some pretty odd faces, not sure what they will look like in game as I have not run one through the tools. If you turn off grid snapping they are constructed very well but as soon as you turn it on and then save/load everything gets snapped. This shows that the current implementation of grid snapping is pretty useless if used as a toggle. I am looking into some painless ways to deal with this (from a users point of view)

Changed
-The current selection is no longer de-selected when a texture directory is loaded.
-The default increment for texture manipulation on patches was change to 0.05 from 0.10. You can also type in much smaller or larger increments into the dialog which will then be used.

Fixed
-Crash when using the clipper on a curve.



Build 161 changes
-----------------
Added
-Edit | Load Prefab... This was there but it now uses .pfb as the default extension for prefabs. MAKE SURE and set the full prefab path you want to use in prefs.
-Edit | Save selection as Prefab... This saves the current selection as a prefab. Defaults to the pref'd prefab location and .pfb extension.

I am working on a preview window that will be used for these and misc_models. Look for that in a few days.

I also discussed grouping (i.e. func_groups) with John a visit or so ago and the current plan is to add key/val pairs to brushes and patches and do away with func_groups. This will allow a lot of flexibility for grouping (hiearchial, named things) as well as other areas too. I will be presenting the overall plan to John soon and if we go with that then this will allow the prefabs to be grouped problem etc.



Build 157 changes
-----------------
Fixed
-misc_model bounding box size on intitial creation
-copying/pasting a misc_model did not update the active edit entity
-misc_models will not continually try to load if the MD3 is not found

Added
-Origin drawing on misc_models, this is what is snapped to grid 
-Pref to turn off 'Show Shader' checkbox
-Show Shader checkbox to prefs
-Shift-A with a selected face selects all brushes that 'contain' that faces texture 

Known
-Assigning a new model to a rotated misc_model screws the bounding box up

Build 156 changes
-----------------
Fixed
-Texture replacement bug when regioning.

Build 155 changes
-----------------
MAJOR changes internally with this version. Make sure everything is backed up. There should not be anything destructive in it but this is a good precaution. There may still be some memory leaks from the new patch storage stuff, those will be addressed in 156.

Changed
-Patch storage, no longer fixed size, unlimited patches.

Fixed
-Undo, should now work in many more cases, and with patches. (this has been tested the least)
-Cap texturing. Worked in my test cases. If you get a cap that does not texture properly I added
|a key combo for cycling the axis face it textures against. Ctrl+Shift+N does this. 

Added
-OpenGL Lighting, turn it on via the view menu. Provides consistent shading/preview between brushes/patches
-New vertex handling, not enabled at present. Still tweaking.
-SHIFT-A selects all entities based on the current selected entity.

Build 154 changes
-----------------
Fixed
-Another instance of entity copy/crash across maps.

Build 153 changes
-----------------
Changed
-"sound" to "noise"
-Show Entities toggle does not fool with func_group'd stuff

Build 152 changes
-----------------
Fixed
-Copy/Paste crash
-Copying brushes across a new map load no longer continually copies them on each subsequent new map


Build 151 changes
-----------------
Added
-Shift clicking a shader in the texture window now spawns "EditPad" and places the cursor at the apporpriated place.

-Patch Inspector (Shift-S) allows access to all patch data and dups surface patch handling. Only works with one patch at a time. If mutliples are selected it uses the latest. This is not quite finished but works.

-Sound player in the Sound Open Dialog (entity window)

Fixed
-Crash when 'Capping' a brush.

Fun Stuff in progress
- MD3 -> Patches. Drop an entire MD3 as a grouped set of patches.

Build 150 changes
-----------------
Added
-Texture show all command.
|Behaviour is as follows:
| 1. Show in use, shows in use.
| 2. Show all, shows everything.
| 3. Showing a particular path, only shows textures in that path. Show All or In Use 
| can be used to un-narrow the view.

Ctrl-A is the show all hotkey.


Build 147-149 changes
---------------------
Fixed
-misc_model entity preview bug
-Texture show in use now works properly
-a few shader issues


Build 146 changes
-----------------
Fixed
- Copy/Paste entities across maps crash
- A ton of memory leaks

Added
- Shift-TAB works in bend mode
- Pointfile is removed on file saves

Build 145 changes
-----------------
Fixed
- misc_models no longer shift during save/load
- misc_models copy/paste correctly
- misc_models paint when first assigned
- assigning a new model name to misc_models works

Added
- Shaders have white border in texture palette
- New shader test code based on the fact that shaders are not directory oriented. On by default, to return
| to the old method, turn "Shader Test" off in prefs and restart. This behaviour loads all of the shaders
| at load time and provides the ability to turn off shader display in the palette. The editor MUST FIND
| qer_editorimage in order to work properly with these (this is true for the old stuff too). This should 
| fix the shader xref problems.


Build 139 changes
-----------------
Fixed
- MD3's are now origin'd properly with their bounding boxes
- Plugin texture load bug

Added
- Texture quality slider (prefs). This can drastically reduce memory and increase speed at the expense of visual quality.
- Shader support for MD3's, proper shader is pulled per surface (assuming it is tga)
- Multple MD3's can be assigned on a per entity basis. Seperate them with a ';'
| i.e. model=models/players/medium/visor/head.md3;models/players/medium/visor/lower.md3; etc.
- Added 'frame=' specifier (0 based) to entity preview to use a particular frame for preview
| This is still a bit fubar'd
- Subdirectory map support ala Raven
- 'color' entity setting ala Raven
- Shader 'qer_editorimage' textures are now load deferred until their parent texture directory is loaded
- Full Plugin support
- You can now override surface dialog names with a plugin (not much use for q3)

Changed
- After assigning a model or sound, focus is given back to XY
- If you right click and add a misc_model, the entity window is automatically
| switched to and the add model dialog is activated
- Models no longer try to load if they fail once
- Prefs layout. Still ugly.


Build 138 changes
-----------------
Fixed
- qer_editorimage behaviour

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