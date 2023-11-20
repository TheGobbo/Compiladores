program funcao (input, output);
var x : integer;
function f(a : integer; var b: integer) : integer;
begin
   x := a + b;
   b := a + 1;
   write(a, b);
   f := x;
end;

begin
   x := 1;
   write(f(f(f(f(1, x), x), x), 1));
   write(x);
end.
