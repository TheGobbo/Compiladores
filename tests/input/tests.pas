program funcao (input, output);
var m : integer;
function f(n: integer; var k : integer): integer;
// function f(n: integer; a: integer; var k : integer): integer;
begin
//    a := 1;
   n := n + 1;
   k := k + n;
   write(n, m);
   f := n;
end;

begin
    m := 0;
    // write(f(1, f(1, 0, m), m));
   write(f(f(f(1, m), m), m));
//    write(f(m, f(m, f(m, 3))));
    // m := 0;
   write(m);
end.
