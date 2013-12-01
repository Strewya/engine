require "lfs";

function GetFileLoc(file)
    print(file);
    file = io.open(file);
    local count = 0;
    for line in file:lines() do
        count = count + 1;
    end;
    file:close();
    return count;
end;

function GetDirLoc(path, tmasks, texts, tdirs)
    local locCount = 0;
    local fileCount = 0;
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local filePath = path .. '/' .. file;
            local attr = lfs.attributes(filePath);
            assert (type(attr) == "table");
            if attr.mode == "directory" and tdirs ~= nil then
                for i, folder in ipairs(tdirs) do
                    if folder == file then
                        local a, b = GetDirLoc(filePath, tmasks, texts);
                        locCount = locCount + a;
                        fileCount = fileCount + b;
                    end;
                end;
            elseif attr.mode == "file" and tmasks ~= nil and texts ~= nil then
                for i, mask in ipairs(tmasks) do
                    for i, ext in ipairs(texts) do
                        if file:sub(file:len() - ext:len() + 1) == ext and file:sub(1, mask:len()) == mask then
                            locCount = locCount + GetFileLoc(filePath);
                            fileCount = fileCount + 1;
                        end;
                    end;
                end;
            end;
        end;
    end;
    return locCount, fileCount;
end;

local path = "D:/My Documents/Visual Studio 2010/Projects/StrujaGames/Snake";
local exts = { ".cpp", ".h", ".xml" };
local masks = { "SG", "winmain", "Common", "forms", "templates", "fonts", "game", "resources", "pong" }
local dirs = { "actions", "fonts", "textures", "sounds" };
local loc, files = GetDirLoc( path, masks, exts, dirs );
print( "loc=" .. loc .. ", files=" .. files );