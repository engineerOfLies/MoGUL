<H1> MoGUL </H1>
MoGUL - Mosaic Game Utility Library.  A suite of libraries designed to facilitate game programming.  Taken as a whole it will be a comprehensive game engine library.

<H2>Usage Documentation</H2>
[Wiki](https://github.com/engineerOfLies/MoGUL/wiki)

<H2>Code Documentation</H2>
[dox](http://engineeroflies.github.io/MoGUL/index.html)

<H2>Libraries</H2>
MoGUL is a collection of libraries that can be used as a whole or taken in parts.  The libraries are broken down into libraries that can be used independently from the rest of MoGUL, the core set of game related libraries and optional extension libraries.
<H3>Stand Alone</H3>
These libraries provide useful utility for the C language that the rest of MoGUL will use, but these are not dependent on the rest of MoGUL.
 - mgl_types - [complete] - a library of type aliases.  Mostly bult from the SDL standard types, but also provides some common types that will be useful in games in general such as vectors (the math construct, not the linked list) and matrices. mgl_dict is a complex generic type that will work well storing config data loaded from JSON, XML or YAML
    - mgl_dict - [complete] - a dictionary type primary used for supporting config
    - mgl_text - [complete] - support for finite length strings.  Used frequently in config
 - mgl_logger - [complete] - a simple logger with support for different logging levels.  Support for loggin in a separate thread.
 - mgl_resource - [complete] - a resource manager "class" that allows the automatic tracking of use and cleaning up of any type of "resource" such as image or audio files or in-project constructs such as entities and windows.
 - mgl_config - [complete] - a config file parser and resource manager.  Will support xml, json and yaml when complete.  Converts config files to MglDict.
    - mgl_json_parse - [complete] - translation from libjansson json_t to mgl_dict
    - mgl_yaml_parse - [complete] - yaml parser using libyaml, parses it to mgl_dict
    - mgl_xml_parse - [in progress] - xml parser using expat, parses it to mgl_dict

<H3>MoGUL Graphics</H3>
These will be the core graphics libraries of MoGUL.  This will contain resource managers for resources typically used in game programming.
 - mgl_graphics - [complete] - initializes and configures rendering pipleline and provides an interface to do basic drawing and frame timing.
 - mgl_draw - [complete] - provides drawing of shape primitives and integration with Chipmunk physics engine.
 - mgl_sprite - [complete] - provides a resource manager for animated sprites.  Sprites can be setup through sprite sheets or through image sequences.
 - mgl_svg - provides a resource manager for scalable vector graphics.  SVG files will be parsed into MglDict and used to draw.
 - mgl_actor - [complete] - provides a context for timing sprite/drawing animations, setting animation sequences and triggering events times with key frames.
 - mgl_font - [complete] - provides a resource manager for loading and displaying fonts / support for word wrapping and font effects.
 
<H3>Extensions</H3>
 - mgl_input - provides a platform to customize user controls and handle control events.
    - mgl_combo - provides support to parsing and catching input chains for complex inputs.
 - mgl_audio - [complete] - provides a resource manager for playing audio clips and music.  Wraps SDL_Mixer.
    - mgl_sound - provides support to load and play sound chunks.  Support for channel groups
    - mgl_music - provides support to load and play music files.  Support for playlists
 - mgl_entity - provides a resource manager for in-game entities.  Can be extended with user data and user defined function pointers.
 - mgl_level - provides a common platform for developing game levels.  Will support a range of level styles from tiled to free form as well as multiple layers and parallax effects.
    - mgl_parallax - provides logic to render parallax layered backgrounds
    - mgl_tiles - provides a set of functions for loading and dealing with tile maps.  Provides the framework for making more robust tiles (animated, active).  Will work with the mgl_path seamlessly.
    - mgl_path - provides a threaded path finding system.  Works with tiles, platforms and free form levels.
 - mgl_gui - provides a graphical user interface platform including resource managers for windows and widgets.
    - mgl_window - provides the resource manager for windows
    - mgl_component - provides the resource manager for building window component widgets
 - mgl_predict - provides a pattern predition implementation.  Useful for AI prediction of player actions.
 
<H2>Coding Style Guide</H2><p>
    CONSTANTS_OR_DEFINES<p>
    \__INCLUDE_GUARDS\__<p>
    MglTypeDefinitions<p>
    mglVariables<p>
    mgl_class_function_format  Where class will be the name of the "class" or file that it is defined in<p>
    /* */Comments will use strict ansii C style  <p>
    /** \@brief documentation comments follow doxygen format <p>

<H2>License</H2><p>
<h3>The MIT License (MIT)</h3>
 @copyright Copyright (c) 2015 EngineerOfLies
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 
