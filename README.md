# What is WUI?

Wui (Web UI) is a proof of concept wrapper for a modular html based UI system. In essence you write your UI in html/css What have you and the library renders it to a graphic for you to include it in your render process.
Includes input handling and confirmation.

Includes automatic build scripts to download the most recent stable versions of CEF, compile it for you, and include all necessary resource.

The build scripts includes a frame for windows and mac but was not yet tested.

Tested (2023-09-22) on:
Arch Linux x86_64, Kernel 6.2.10-arch1-1, X11

# Usage:

Opening this repo in VSCode will load a few tasks that may be used for compilation. All of them should work out of the box if you have all build requirements.

run "setup build", and then "build". ( for now only debug builds are hard coded)

# Build Requirements:

TODO

# Caveats:

It is not possible to infer from general html code if an area has any listeners associated to it. Therefore any interaction is matched against the currently displayed graphic. If the pixel on which the action occured is completely empty (RGBA values all 0) then the click is ignored by the ui (and the associated function returns false).
You may specify 'force' flag to, well, force the event to be processed anyways.

MoveEvents should always be forwarded to the UI or else hover effects may break. Technically the confirmation here can be used if "any ui is hit" but for now its void.

# TODO: (out of date)

- make custom scheme so backend can be navigated more cleanly (html things)
- setup development set for some data
- Fix renderer to only convert pixel data in designated rect sizes and not full buffer (data bottleneck)
- parse mouse input modifiers, strg/ctrl/shift click
- parse windows specific system keys?
- forward key events to the UI, and back if interaction wasn't consumed
- make the renderer of allegro independent thread from main (create display in that new thread and manage it internally)
- handle mouse wheel events
- separate game object management from the renderer
- Smarter object instantiation and backend Rendering things in general (make some kind of demo game out of this)
- Reactive window resizing and adjustment of backbuffer and pipeline
- Work out JS <-> Engine bidirectional binding and function subscriptions, event based system
  - JS and Engine should both be able to subscribe to events sent from the other party in some generalistic manner
  - most likely use the global window object since that can be injected cleanly.
- hot reloading test
- complex css styling test
- Reactive, compressed, webpack, jest, etc compatability test (though this should all pass)

# Author:

Dominik Völkel
