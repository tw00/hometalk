function result = read_ir_signals(filename)
    fid = fopen(filename);
    tline = fgetl(fid);
    result = struct('code',{},'bits',{},'type',{},'raw',{},'length',{});
    i = 0; k = 1;
    while ischar(tline)
        i = i + 1;
        switch(mod(i-1,3)+1)
            case 1
                result(k).code = tline;
            case 2
                temp = textscan(tline,'%s %s %s (%d bits)');
                result(k).type  = temp{2}{1}(1:end-1);
                %result(k).code2 = temp{3}{1};
                result(k).bits  = temp{4};
            case 3
                c_str = regexp(tline,':','split'); 
                scan1 = textscan(c_str{1}, 'Raw (%d)');
                scan2 = textscan(c_str{2}, '%d');
                result(k).length = scan1{1};
                result(k).raw = scan2{1};
                % result(k)
                k = k + 1;
        end
        tline = fgetl(fid);
    end
    fclose(fid);
end

