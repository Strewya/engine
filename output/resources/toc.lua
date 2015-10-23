return {
    textures = {
        [1] = "resources/Textures/rainbowland_atlas.tif",
        [2] = "resources/Textures/background.png",
        [3] = "resources/Textures/font_t.png"        
    },
    vertexShaders = {
        [1] = {path="resources/Shaders/shader_vs.cso", vertex=1}
    },
    pixelShaders = {
        [1] = "resources/Shaders/shader_ps.cso",
        [2] = "resources/Shaders/health_ps.cso"
    },
    sounds = {
        [1] = "resources/Sounds/reload.wav",
        [2] = "resources/Sounds/pistol.wav",
        [3] = "resources/Sounds/shotgun.wav",
        [4] = "resources/Sounds/uzi.wav",
        [5] = "resources/Sounds/sniper.wav",
        [6] = "resources/Sounds/missile.wav",
    },
    meshes = {
    },
    materials = {
        [1] = {vertex = 1, pixel = 1, texture = 1},
        [2] = {vertex = 1, pixel = 1, texture = 2},
        [3] = {vertex = 1, pixel = 1, texture = 3},
        [4] = {vertex = 1, pixel = 2, texture = 1},
    },
    fonts = {
        [1] = {path="resources/Defs/font.font", material=3}
    },
    sprites = {
    }
};