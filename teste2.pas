program comandoWhile(input, output);
var n, k : integer;
    f1, f2, f3 : integer;
    b : boolean;
begin 

    b := true;
    write(b);
    b := false;
    write(b);

    read(n);
    f1 := 0; f2 := 1; k := 1;
    while (k <= n) do
    begin 
        f3 := f2 + f1;
        f1 := f2;
        f2 := f3;
        k := k + 1;
    end;
    write(n, k);
end.

