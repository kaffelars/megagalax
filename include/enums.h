enum class sceneids
{
    mainmenu, settings, game, loadsystem, blank
};

enum class shaderids
{
    tempparticle, line, particlegas, particlesimple, particlestars, particlecircles, particlegasstars,
    compute_gravity_merge, compute_moveparticles, compute_gravity, compute_moveparticles_merge, compute_gravity_interact, compute_moveparticles_interact,
    compute_gravity_simple, compute_moveparticles_simple
};

enum class shadertype
{
    fragvert, compute
};

enum class warningseverity
{
    information, mild, moderate, severe
};

enum class boxtype
{
    okaybox, okaycancelbox
};

enum class setting_enum
{
    screen_x, screen_y, fov, vsync, uiscale, mousesens, simulateinbg, cameramode, invert_y, movespeed, shaderworkgroups, simulateeveryn
};

enum class key_type
{
    keyboard,
    mousebutton,
    mousescroll
};

enum class key_enum
{
    forwardkey,
    backwardkey,
    up,
    down,
    left,
    right,
    movecamera,
    zoomin,
    zoomout,
    select,
    toggle_pause,
    escape,
    toggle_camera,
    last //for count
};

enum class key_status
{
    down,
    up
};

enum class key_event
{
    clicked,
    held,
    released
};
