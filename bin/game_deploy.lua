
local dep = {};

function dep:deploy(game, resourceList)
	self:setupPaths(game);
	print("Deploying " .. game .. " game to " .. nixPath(self.dstRootDir));
	local files = {};
	--copy bin folder: exe and DLLs
	forEachDir(self.srcBinDir, function(p, f, c)
		local src = p..'/'..f;
		local dst = self.dstBinDir..'/'..f;
		local file = copyIfNewer(src, dst);
		if(file) then
			table.insert(c, file);
		end;
	end, files);
	
	--copy required resource files
	for i,v in ipairs(resourceList) do
		local src = self.srcResDir..'/'..v;
		local dst = self.dstResDir..'/'..v;
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
	forEachDir(self.dstResDir, self.cleanUnusedFiles, self.prefixNames(resourceList, self.dstResDir.."/"), files);
	if(#files > 0) then
		print("Files deleted: " .. #files);
		for _,v in ipairs(files) do
			print(v);
		end;
	end;
end;

function dep:setupPaths(game)
	local dropboxDir = os.getenv("USERPROFILE").."/AppData/Roaming/Dropbox/host.db";
	ensure(checkFileExists(dropboxDir), "Dropbox not installed!");
	dropboxDir = findLineContaining(dropboxDir, 2);
	
	self.dstRootDir = self:b64dec(dropboxDir) .. "/Games/" .. game;
	self.dstBinDir = self.dstRootDir .. "/bin";
	self.dstResDir = self.dstRootDir .. "/resources";
	self.srcBinDir = gProjDir .. "/output/Final";
	self.srcResDir = gProjDir .. "/output/resources";
end;

function dep.prefixNames(tbl, prefix)
	local newTbl = {};
	for i,v in ipairs(tbl) do
		table.insert(newTbl, prefix .. v);
	end;
	return newTbl;
end;



function dep.cleanUnusedFiles(path, file, usedFilesList, deletedFiles)
	local f = path..'/'..file;
	if(not findInTable(usedFilesList, f)) then
		delete(f);
		table.insert(deletedFiles, f);
	end;
end;

function dep:b64dec(data)
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

return dep;