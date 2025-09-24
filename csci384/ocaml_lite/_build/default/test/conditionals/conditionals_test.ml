open OUnit2

let _ = Printf.printf "Booleans/Parse\n"
let _ = run_test_tt_main Parser.bool_expr_tests
let _ = Printf.printf "Booleans/Typecheck\n"
let _ = run_test_tt_main Typecheck.expressions
let _ = Printf.printf "Booleans/Interpret\n"
let _ = run_test_tt_main Interpreter.expressions
let _ = Printf.printf "Conditionals/Parse\n"
let _ = run_test_tt_main Parser.if_tests
let _ = Printf.printf "Conditionals/Typecheck\n"
let _ = run_test_tt_main Typecheck.if_expr
let _ = Printf.printf "Conditionals/Interpret\n"
let _ = run_test_tt_main Interpreter.if_exprs
