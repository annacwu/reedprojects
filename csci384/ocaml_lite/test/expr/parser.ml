open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Ast

let constant_tests =
  "constants"
  >::: List.map make_expr_ast_str [ ("integer", EConst (CInt 24), "24") ]

let precedence_tests =
  "precedence"
  >::: List.map make_expr_str_str
         [
           ("~ higher prec than *", "(~ 1) * 2", "~ 1 * 2");
           ("*, /, mod equal prec", "((1 * 2) / 3) mod 4", "1 * 2 / 3 mod 4");
           ("mod, /, * equal prec", "((1 mod 2) / 3) * 4", "1 mod 2 / 3 * 4");
           ("* higher prec than +", "1 + (2 * 3)", "1 + 2 * 3");
           ("+, - equal prec", "(1 + 2) - 3", "1 + 2 - 3");
           ("-, + equal prec", "(1 - 2) + 3", "1 - 2 + 3");
         ]

let associativity_tests =
  "associativity"
  >::: List.map make_expr_str_str
         [
           ("plus", "(1 + 2) + 3", "1 + 2 + 3");
           ("minus", "(1 - 2) - 3", "1 - 2 - 3");
           ("times", "(1 * 2) * 3", "1 * 2 * 3");
           ("div", "(1 / 2) / 3", "1 / 2 / 3");
           ("mod", "(1 mod 2) mod 3", "1 mod 2 mod 3");
         ]

let expr_basic_tests =
  "basic"
  >::: List.map make_expr_ast_str
         [
           ("binop", EBinop (EConst (CInt 1), BAdd, EConst (CInt 2)), "1 + 2");
           ("unop", EUnop (UNegate, EConst (CInt 1)), "~1");
           ("paren", EConst (CInt 2), "(2)");
         ]

let expr_tests =
  "expressions"
  >::: [
         constant_tests; expr_basic_tests; associativity_tests; precedence_tests;
       ]

let parse_tests = "test suite for parsing" >::: [ expr_tests ]
