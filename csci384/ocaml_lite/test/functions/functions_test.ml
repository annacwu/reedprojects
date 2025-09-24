open OUnit2

let _ = Printf.printf "Functions/Parse\n"
let _ = run_test_tt_main Parser.parse_tests
let _ = Printf.printf "Functions/Typecheck\n"
let _ = run_test_tt_main Typecheck.all_functions
let _ = Printf.printf "Recursion/Tyepcheck\n"
let _ = run_test_tt_main Typecheck.all_recur
let _ = Printf.printf "Polymorphism/Tyepcheck\n"
let _ = run_test_tt_main Typecheck.polymorphism
let _ = Printf.printf "Functions/Interpret\n"
let _ = run_test_tt_main Interpreter.interp_tests
let _ = Printf.printf "Recursion/Interpret\n"
let _ = run_test_tt_main Interpreter.all_recur
