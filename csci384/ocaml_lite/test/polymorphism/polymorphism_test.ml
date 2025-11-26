open OUnit2

let _ = Printf.printf "Polymorphism/Parse\n"
let _ = run_test_tt_main Parser.parse_tests
let _ = Printf.printf "Polymorphism/Typecheck\n"
let _ = run_test_tt_main Typecheck.top_level
