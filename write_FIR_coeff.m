%% write fir bandpass coeff
a = coeff
filename = 'FIR_bandpass.c';
file_ID = fopen(filename, 'a');
step = 527;
for i = 1:step 
    tmp = [];
    for y = i:step:length(a)
        if y == i
            % first one
            tmp = a(y);
        elseif (y) <= (length(a))
            % second, third, fourth
            tmp = [tmp, a(y)];          
        end
    end
    if length(tmp) < 4
        while length(tmp) < 4
            tmp = [tmp, 0.];
        end 
    end
    write_coeff(file_ID, 'FIR_bandpass', i, tmp, length(tmp));
    clear tmp
end

fclose(file_ID);