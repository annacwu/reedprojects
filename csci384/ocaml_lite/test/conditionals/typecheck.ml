open OUnit2
open Ocaml_lite_test_utils.Utils

let constants =
  "constants"
  >::: List.map tc_expr
         [
           ("int", "2", Some (Int));
           ("true", "true", Some (Bool));
           ("false", "false", Some (Bool));
         ]

let operators =
  "operators"
  >::: List.map tc_expr
         [
           ("plus pass", "3 + 4", Some (Int));
           ("plus arg fail", "2 + true", None);
           ("minus pass", "3 - 4", Some (Int));
           ("minus arg fail", "2 - true", None);
           ("lt pass", "3 < 4", Some (Bool));
           ("lt arg fail", "() < 5", None);
           ("eq pass int", "3 = 4", Some (Bool));
           ( "eq pass bool",
             "true = true",
             Some (Bool) );
           ("eq arg fail", "1 = ()", None);
         ]

let if_expr =
  "if"
  >::: List.map tc_expr
         [
           ( "pass",
             "if true then 2 else 3",
             Some (Int) );
           ("bad condition", "if 1 then 2 else 3", None);
         ]

let expressions = "expressions" >::: [ constants; operators ]
