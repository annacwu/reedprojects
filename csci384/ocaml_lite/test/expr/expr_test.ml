open OUnit2

let _ = Printf.printf "Arithmetic/Parse\n"
let _ = run_test_tt_main Parser.parse_tests
let _ = Printf.printf "Arithmetic/Interpret\n"
let _ = run_test_tt_main Interpreter.interp_tests
