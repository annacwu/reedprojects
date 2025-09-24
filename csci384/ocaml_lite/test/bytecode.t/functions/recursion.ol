let _ = print_string (string_of_int (
  let rec fact x = if x = 0 then 1 else x * fact (x - 1) in
  fact 5
));;
