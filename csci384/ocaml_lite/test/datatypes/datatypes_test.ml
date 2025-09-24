open OUnit2

let _ = Printf.printf "Datatypes/Parse\n"
let _ = run_test_tt_main Parser.parse_tests
let _ = Printf.printf "Datatypes/Typecheck\n"
let _ = run_test_tt_main Typecheck.tc_tests
let _ = Printf.printf "Datatypes/Interpret\n"
let _ = run_test_tt_main Interpreter.interp_tests
