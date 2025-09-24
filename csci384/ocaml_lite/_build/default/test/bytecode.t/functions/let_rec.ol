let rec fact (x : int) : int =
  if x = 0
  then 1
  else x * fact (x - 1) ;;

let _ = print_string (string_of_int (fact 5)) ;;
