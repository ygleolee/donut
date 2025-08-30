This program animates a rotating floating donut using ascii characters in the terminal. Users can control camera location, lighting, rotation, and more using the keyboard.

## Requirements

- c++23
- [xmake](https://xmake.io)
- termios (should be in most unix machines by default)
- a terminal emulator that supports ANSI escape codes

## Installation

- install [xmake](https://xmake.io) if you haven't
- `git clone --recurse-submodules https://github.com/ygleolee/donut`: clone the repository and dependencies
- `xmake build donut`: build the binary
- `xmake install donut`, use `-o <path>` to install it to somewhere else. If you run into permission issues, try `--admin`.
- done!

## Usage

- `donut` to run in basic mode (no user control)
- `donut -i` to run in interactive mode (control the animation via the keyboard). See the available keymaps [here](#default-configuration).
- `donut -c` to specify a [configuration file](#configuration)

## Configuration

The configuration is determined in the following precedence:
1. configuration file passed with `-c`
1. `DONUT_CONFIG_PATH` environment variable
1. `~/.config/donut/config.toml` if it exists
1. default configuration

Configuration is specified in a `.toml` file. Please refer to the [default configuration](#default-configuration) below for details about each option.

Very importantly, there is one (and only one) parameter that must be manually set for the animation to look reasonable. Since the program uses ascii characters as pixels, the height:wid ratio of a "character" in the user's terminal is crucial for the animation. As far as I know, there is no programmatic way to get this value, so please set it manually using the `CHAR_RATIO` environment variable (or via the configuration file, but the environment variable takes precedence when both are set). Values are normally between 1 and 2. The default value($1.71$) should be close enough to most people's terminal, but a slight difference to the true value will make the animation look weird.

### Default Configuration
```toml
[animation]
buffer_size = 50 # size of the animation buffer
fallback_keep = 5 # how many frames to keep in the buffer when the user hits a key that invalides the frames (ex. change of lighting)

[shape]
delta = 0.2 # the amount (in radian) to increase/decrease for each key press
rps = [ 0.508, 0.384, 0.693 ] # the initial rotating speed (radians per second) along the three axes

[camera] # controls camera movement parameters
# the screen is the XY-plane, and the viewer/camera is on the Z-axis
max = 800.0 # maximum z value the camera can be at (farthest from the donut)
min = 200.0 # minimum z value the camera can be at (closest to the donut)
steps = 12 # how many differet camera positions to allow between min and max, the program automatically determines the step sizes so that visually it looks linear

[contorl]
debounce = 50 # user input debouncing in milliseconds (i.e. how many milliseconds before the next keystroke counts)

[display]
char_ratio = 1.71 # the height:width ratio for a "character" in the terminal, please do set this manually
fps = 30 # frames per second
grayscale = '.......,,,,,~~;;==**#$@ ' # the ascii grayscale
range = 100.0 # don't change this for now

[light] # there are two types of lighting available a parallel source (like the sun) or a point/radial source (like a light bulb)
type = 'PARALLEL' # either PARALLEL or POINT
parallel = [ 1.0, -4.0, -4.0 ] # for parallel lighting, set its direction as a vector
point = [ 100.0, 40.0, 300.0 ] # for point lighting, set its position
rps = [ 0.0, 0.0, 0.0 ] # rotation speed (radians per second) for the light (the vector for PARALLEL and the point on the sphere it was on originally for POINT)
rpp = 0.1 # lighting to rotate (note that this directly changing the direction of the light, which is different from delta for shape rotation)

[keymap] # sets keymaps
CAMERA_BACKWARD = 'k' # moves the camera backward one step
CAMERA_FORWARD = 'j' # moves the camera forward one step
INC_SHAPE_ROT_X = 'x' # increase the shape's rotation speed along the X-axis by delta
INC_SHAPE_ROT_Y = 'y' # increase the shape's rotation speed along the Y-axis by delta
INC_SHAPE_ROT_Z = 'z' # increase the shape's rotation speed along the Z-axis by delta
DEC_SHAPE_ROT_X = 'X' # decrease the shape's rotation speed along the X-axis by delta (if it goes negative then it's just rotating the other way)
DEC_SHAPE_ROT_Y = 'Y' # decrease the shape's rotation speed along the Y-axis by delta (if it goes negative then it's just rotating the other way)
DEC_SHAPE_ROT_Z = 'Z' # decrease the shape's rotation speed along the Z-axis by delta (if it goes negative then it's just rotating the other way)
LIGHT_TOGGLE_TYPE = 'p' # toggle between parallel/point lighting
INC_LIGHT_ROT_X = 'w' # rotate the light by rpp amount along the X-axis
INC_LIGHT_ROT_Y = 'a' # rotate the light by rpp amount along the Y-axis
INC_LIGHT_ROT_Z = 'q' # rotate the light by rpp amount along the Z-axis
DEC_LIGHT_ROT_X = 's' # rotate the light by rpp amount along the X-axis (negative direction)
DEC_LIGHT_ROT_Y = 'd' # rotate the light by rpp amount along the Y-axis (negative direction)
DEC_LIGHT_ROT_Z = 'e' # rotate the light by rpp amount along the Z-axis (negative direction)
NEXT_FRAME = 'f' # display the next frame (when animation is paused)
RESET_TO_DEFAULT = 'r' # reset to the default configuration
START_PAUSE = ' ' # pause/start the animation
ZERO_ROTATION = '0' # stop the shape's rotation

# these don't work yet
SHOW_CONFIG = 'C'
EXPORT_CONFIG = 'E'
COMMAND_HISTORY = 'h'
```

## Future
- add different shapes
- add some way to export parameters
- output the current parameters to the screen
