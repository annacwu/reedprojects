open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Interpret

let constants =
  "constants"
  >::: List.map interp_expr_test
         [ ("bool", failwith "Fill in with value 'false'", "false") ]

let operators =
  "operators"
  >::: List.map interp_expr_test
         [
           ("lt true", failwith "Value 'true'", "0 < 1");
           ("eq int", failwith "Value 'true'", "0 = 0");
           ("eq bool", failwith "Value 'true'", "false = false");
           ("and true", failwith "Value 'true'", "true && true");
           ("or true", failwith "value 'true'", "false || true");
           ("not true", failwith "Value 'false'", "not true");
         ]

let if_exprs =
  "if expressions"
  >::: List.map interp_expr_test
         [ ("if true", VInt 1, "if true then 1 else 2") ]

let expressions =
  "expressions"
  >::: [
         constants;
         operators;
         expr_fail "unassigned value" "Evaluated unassigned value" "x + 2";
       ]
