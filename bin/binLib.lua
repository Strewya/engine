require('lfs');

function forEachDir(path, fn, ...)
	for file in lfs.dir(path) do
        if(file ~= "." and file ~= "..") then
			local f = path .. '/' .. file;
			local attr = lfs.attributes(f);
			if(attr.mode == "directory") then
                forEachDir(f, fn, ...);
            else
                fn(path, file, ...);
            end;
		end;
	end;
end;

function winPath(path)
	return path:gsub("/", "\\");
end;

function nixPath(path)
	return path:gsub("\\", "/");
end;

function b64dec(data)
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

function findLine(filename, target)
	local f = io.open(filename, "r");
	local i = 1;
	local retval = "";
	for line in f:lines() do
		if(type(target) == "number" and i == target) then
			retval = line;
			break;
		elseif(type(target) == "string" and line:find(target) ~= nil) then
			retval = line;
			break;
		end;
		i = i+1;
	end;
	f:close();
	return retval;
end;

function must(check, errorTxt)
	if(not check) then
		print(errorTxt);
		os.exit();
	end;
end;