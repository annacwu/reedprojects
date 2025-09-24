let _ = print_string (string_of_int (
  let f = let x = 1 in fun y => x + y in
  f 2
));;
