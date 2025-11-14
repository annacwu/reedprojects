open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Interpret

let let_exprs =
  "let expressions"
  >::: List.map interp_expr_test
         [
           ( "recursion",
             VInt 0,
             "let rec f (x : int) : int = if x = 0 then x else f (x - 1) in f 3"
           );
            ("two params", 
            VInt 6, 
            "let rec mul (x : int) (y : int) : int = if x = 0 then 0 else y + mul (x - 1) y in mul 3 2"
            );
            ("three params", 
            VInt 6, 
            "let add (x : int) (y : int) (z: int) : int = x + y + z in add 1 2 3"
            );
            ("no shadowing",
            VInt 2, 
            "let f (x : int) : int = x + 1 in let f (x : int) : int = f x + 1 in f 0"
            );
         ]

let lambda_exprs =
  "lambdas"
  >::: List.map interp_expr_test
         [
           ( "closure",
             VClosure("x", EVar("x"), [], None),
             "fun (x : int) : int => x" );
           ("applying a closure", VInt 3, "(fun (x : int) : int => x + 2) 1");
           ( "context capture",
             VInt 3,
             "let f = let x = 1 in fun (y : int) : int => x + y in f 2" );
           ( "let with fun",
             VInt 2,
             "let f = fun (x : int) : int => x + 1 in f 1"
           );
         ]

let definitions =
  "definition and use"
  >::: List.map program_test
         [
           ( "function definition",
             check_val "y" (VInt 6),
             "let f (x : int) : int = x + 2;; let y = f 4;;",
             "Couldn't refer to defined function" );
            ("closure in top-level", 
             check_val "z" (VInt 10), 
             "let x = 5;; let addx (y : int) : int = x + y;; let z = addx 5;;", 
             "Closure capture failed");
         ]

let recursion =
  "recursive functions"
  >::: List.map program_test
         [
           ( "recursive definition",
             check_val "y" (VInt 120),
             "let rec fact (x : int) : int = if x = 0 then 1 else x * fact (x \
              - 1);; let y = fact 5;;",
             "Couldn't run recursive function" );
            ( "recursive two params definition",
             check_val "z" (VInt 6),
             "let rec mul (x : int) (y : int) : int = if x = 0 then 0 else y + mul (x - 1) y;; \
              let z = mul 3 2;;",
             "Couldn't run recursive function" );
         ]

let builtins =
  "built-in functions"
  >::: List.map program_test
         [
           ( "string_of_int",
             check_val "y" (VString "12"),
             "let y = string_of_int 12;;",
             "string_of_int failed" );
           ( "int_of_string",
             check_val "y" (VInt 12),
             "let y = int_of_string \"12\";;",
             "int_of_string failed" );
            ("classic structure",
            check_val "s" (VString "3"),
            "let s = string_of_int (
            let f = let x = 1 in fun (y : int) : int => x + y in f 2);;",
            "something failed lol"
            );
         ]

let interp_tests =
  "test suite for interpretation"
  >::: [
         lambda_exprs;
         definitions;
         builtins;
         expr_fail "int_of_string fail"
           "int_of_string succeeded on non-int string" "int_of_string \"abc\"";
       ]

let all_recur = "recursion" >::: [ let_exprs; recursion ]
