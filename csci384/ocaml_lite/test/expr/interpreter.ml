open OUnit2
open Ocaml_lite_test_utils.Utils

let constants =
  "constants" >::: List.map interp_expr_test [ ("integer", VInt 2, "2") ]

let operators =
  "operators"
  >::: List.map interp_expr_test
         [
           ("plus", VInt 4, "1 + 3");
           ("minus", VInt 3, "10 - 7");
           ("times", VInt 15, "3 * 5");
           ("divide", VInt 4, "12 / 3");
           ("integer divide", VInt 3, "11 / 3");
           ("mod", VInt 1, "5 mod 2");
           ("neg", VInt (-3), "~3");
         ]

let expressions = "expressions" >::: [ constants; operators ]
let interp_tests = "test suite for interpretation" >::: [ expressions ]
