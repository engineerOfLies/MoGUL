# MoGUL
MoGUL - Mosaic Game Utility Library.  A suite of libraries designed to facilitate game programming.  Taken as a whole it will be a comprehensive game engine library.

<H2>Libraries</H2>
MoGUL is a colleciton of libraries that can be used as a whole or taken in parts.  The libraries are broken down into libraries that can be used independently from the rest of MoGUL, the core set of game related libraries and optional extension libraries.
<H3>Stand Alone</H3>
These libraries provide useful utility for the C language that the rest of MoGUL will use, but these are not dependent on the rest of MoGUL.
 - mgl_types - a library of type aliases.  Mostly bult from the SDL standard types, but also provides some common types that will be useful in games in general such as vectors (the math construct, not the linked list) and matrices.
 - mgl_dict - a dictionay container type for C. Designed to work well when importing data from configuration files like JSON, YAML or XML.  Contains loaders for each of those file formats.
 - mgl_rm - a resource manager "class" that allows the automatic tracking of use and cleaning up of any type of "resource" such as image or audio files or in-project constructs such as entities and windows.

<H3>MoGUL Core</H3>
 - mgl_graphics - initializes and configures rendering pipleline and provides an interface to do basic drawing and frame timing.
 - mgl_shapes - provides drawing of shape primitives and integration with Chipmunk physics engine.
 - mgl_sprite - provides a resource manager for animated sprites.  Sprites can be setup through sprite sheets or through image sequences.
 - mgl_audio - provides a resource manager for playing audio clips and music.  Wraps SDL_Mixer.
 - mgl_actor - provides a context for timing sprite animations, setting animation sequences and triggering events times with key frames.

<H3>Extensions</H3>
 - mgl_tiles
 - mgl_path - provides a threaded path finding system.  Works with paths or 

<H2>Documentation</H2>
URL: TBD

<H2>Coding Style Guide</H2><p>
CONSTANTS_OR_DEFINES<p>
\__INCLUDE_GUARDS\__<p>
MglTypeDefinitions<p>
mglVariables<p>
mgl_class_function_format  Where class will be the name of the "class" or file that it is defined in<p>
/* Comments will use strict ansii C style\*/<p>
/** @brief documentation comments follow doxygen format \*/<p>
