return {
    textures = {
        [AssetID.AssetTextureAtlas] = "resources/Textures/rainbowland_atlas.tif",
        [AssetID.AssetTextureBackground] = "resources/Textures/background.png",
        [AssetID.AssetTextureFont] = "resources/Textures/font_t.png"        
    },
    vertexShaders = {
        [AssetID.AssetVertexShaderMain] = {path="resources/Shaders/shader_vs.cso", vertex=1}
    },
    pixelShaders = {
        [AssetID.AssetPixelShaderMain] = "resources/Shaders/shader_ps.cso",
        [AssetID.AssetPixelShaderHealth] = "resources/Shaders/health_ps.cso"
    },
    sounds = {
        [AssetID.AssetSoundReload] = "resources/Sounds/reload.wav",
        [AssetID.AssetSoundPistol] = "resources/Sounds/pistol.wav",
        [AssetID.AssetSoundShotgun] = "resources/Sounds/shotgun.wav",
        [AssetID.AssetSoundUzi] = "resources/Sounds/uzi.wav",
        [AssetID.AssetSoundSniper] = "resources/Sounds/sniper.wav",
        [AssetID.AssetSoundMissile] = "resources/Sounds/missile.wav",
    },
    meshes = {
    },
    materials = {
        [AssetID.AssetMaterialWorld] = {vertex = AssetID.AssetVertexShaderMain, pixel = AssetID.AssetPixelShaderMain, texture = AssetID.AssetTextureAtlas},
        [AssetID.AssetMaterialFont] = {vertex = AssetID.AssetVertexShaderMain, pixel = AssetID.AssetPixelShaderMain, texture = AssetID.AssetTextureFont},
        [AssetID.AssetMaterialBackground] = {vertex = AssetID.AssetVertexShaderMain, pixel = AssetID.AssetPixelShaderMain, texture = AssetID.AssetTextureBackground},
        [AssetID.AssetMaterialHealth] = {vertex = AssetID.AssetVertexShaderMain, pixel = AssetID.AssetPixelShaderHealth, texture = AssetID.AssetTextureAtlas},
    },
    fonts = {
        [AssetID.AssetFontMain] = {path="resources/Defs/font.font", material=AssetID.AssetMaterialFont}
    },
    sprites = {
    }
};