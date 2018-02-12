lst = seriallist;
s = serial(lst(2));
fopen(s);
for i = 1:100
    fscanf(s)
end
flose(s);