
if(not gProjDir) then
	dofile(os.getenv("SGPROJECT").."/bin/cmn.lua");
end;

local function b64dec(data)
	local b = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
    data = string.gsub(data, '[^'..b..'=]', '');
    return (data:gsub('.', function(x)
        if(x == '=') then return '' end;
        local r, f = '', (b:find(x) - 1);
        for i=6,1,-1 do r = r..(f%2^i-f%2^(i-1)>0 and '1' or '0') end;
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if(#x ~= 8) then return '' end;
        local c = 0;
        for i=1, 8 do c=c+(x:sub(i,i)=='1' and 2^(8-i) or 0) end;
        return string.char(c);
    end))
end;

local function prefixNames(tbl, prefix)
	local newTbl = {};
	for i,v in ipairs(tbl) do
		table.insert(newTbl, prefix .. v);
	end;
	return newTbl;
end;

local function cleanUnusedFiles(path, file, usedFilesList, deletedFiles)
	local f = path..'/'..file;
	if(not findInTable(usedFilesList, f)) then
		delete(f);
		table.insert(deletedFiles, f);
	end;
end;

function dropboxTarget()
	local dropboxDir = os.getenv("USERPROFILE").."/AppData/Roaming/Dropbox/host.db";
	if(checkFileExists(dropboxDir)) then
		dropboxDir = findLineContaining(dropboxDir, 2);
		dropboxDir = b64dec(dropboxDir) .. "/Games/";
	else
		dropboxDir = nil;
	end;
	return dropboxDir;
end;

function deployTarget()
	return gProjDir .. "/deploy/";
end;

function doDeployment(target)
	if(target == nil) then
		print("Invalid deploy target");
		return;
	end;
	local game = gameName or extractGameName();
	local resFile = gProjDir .. "/src/Games/" .. game .. "/resourceList.lua";
	local resDir = gProjDir .. "/output/resources";

	local resList = resourceList and resourceList or dofile(resFile);
	local resOK = allResourcesPresent or assertResourcesPresent(resList, resDir);

	ensure(resOK, "Some resource dependencies are missing! Fix those first!");

	local dstRootDir = target .. game;
	local dstBinDir = dstRootDir .. "/bin";
	local dstResDir = dstRootDir .. "/resources";
	local srcBinDir = gProjDir .. "/output/Final";
	local srcResDir = gProjDir .. "/output/resources";
		
	print("Deploying " .. game .. " game to " .. nixPath(dstRootDir));
	local files = {};
	--copy bin folder: exe and DLLs
	forEachDir(srcBinDir, function(p, f, c)
		local src = p..'/'..f;
		local dst = dstBinDir..'/'..f;
		local file = copyIfNewer(src, dst);
		if(file) then
			table.insert(c, file);
		end;
	end, files);

	--copy required resource files
	for i,v in ipairs(resList) do
		local src = srcResDir..'/'..v;
		local dst = dstResDir..'/'..v;
		local file = copyIfNewer(src, dst);
		if(file) then
			table.insert(files, file);
		end;
	end;
	if(#files > 0) then
		print("Files copied: " .. #files);
		for _,v in ipairs(files) do
			print(v);
		end;
	end;
	files = {};
	--clean unused resource files
	forEachDir(dstResDir, cleanUnusedFiles, prefixNames(resList, dstResDir.."/"), files);
	if(#files > 0) then
		print("Files deleted: " .. #files);
		for _,v in ipairs(files) do
			print(v);
		end;
	end;
end;