open OUnit2

let _ = Printf.printf "Bindings/Parse\n"
let _ = run_test_tt_main Parser.parse_tests
let _ = Printf.printf "Bindings/Typecheck\n"
let _ = run_test_tt_main Typecheck.tc_tests
let _ = Printf.printf "Bindings/Interpreter\n"
let _ = run_test_tt_main Interpreter.interp_tests
