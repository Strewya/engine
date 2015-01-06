return 
{ -- resource dependencies
	"Defs/font.font",
	"Sounds/music.mp3",
	"Sounds/reload.wav",
	"Sounds/shot.mp3",
	"Textures/background.png",
	"Textures/font_t.png",
	"Textures/rainbowland_atlas.png"
},
{ -- DLL dependencies
	"tolua++",
	"lua51",
	"lua5.1",
	Debug = {"fmodL"},
	Release = {"fmod"},
	Deploy = {"fmod"}
};