open OUnit2
open Ocaml_lite.Parser
open Ocaml_lite.Typecheck

let tc_program (inp : string * string * bool) : test =
  let label, prog, pass = inp in
  label >:: fun _ ->
  try
    let _ = typecheck (parse prog) [] in
    assert_bool "program typechecked but shouldn't have" pass
  with
  | TypeError _ -> assert_bool "program failed typechecking" (not pass)
  | OUnitTest.OUnit_failure _ as e -> raise e
  | e -> raise e

let let_expr =
  "let"
  >::: List.map tc_program
         [
           ( "pass function",
             "let x : int -> int = let f (y : int) : int = y in f;;",
             true );
           ( "fail function",
             "let x : unit -> int = let f (y : unit) : int = y in f;;",
             false );
           ( "pass anon function",
             "let f = fun (x : int) : int => x + 1;;",
             true);
           ( "curried type", 
             "let add (x : int) (y : int) : int = x + y;; let f : int -> int -> int = add;;",
             true);
         ]

let rec_let_expr =
  "let rec"
  >::: List.map tc_program
         [
           ( "let rec pass",
             "let _ = let rec f (x : int) : int = if x < 0 then x else f (x - \
              1) in f 3;;",
             true );
           ( "let rec fail",
             "let rec f (x : int) : int = if f (x - 1) < 0 then true else \
              false;;",
             false );
         ]

let lambda_expr =
  "lambda"
  >::: List.map tc_program
         [
           ( "pass with type",
             "let x : int -> int = fun (y : int) : int => y;;",
             true );
            ("simple lambda", 
            "let f = fun (x : int) : int => x + 1;;", 
            true);
         ]

let expressions = "expressions" >::: [ let_expr; lambda_expr ]

let functions =
  "function definitions"
  >::: List.map tc_program
         [ 
          ("id pass", "let f (x : int) : int = x;; let _ = f 2;;", true);
          ("param type error", "let inc (x : int) : int = x && true;;", false);
          ("two params", "let add (x : int) (y : int) : int = x + y;;", true);
          ("two params type error", "let add (x : bool) (y : bool) : int = x + y;;", false);
         ]

let all_functions = "functions" >::: [ expressions; functions ]

let recursion =
  "recursive definitions"
  >::: List.map tc_program
         [
           ( "bad self type",
             "let rec f (x : int) : int = if x < 1 then f x else f true;;",
             false );
            ( "good self type",
             "let rec f (x : int) : int = if x < 1 then x else f (x - 1);;",
             true );
            ( "bad type instructions example", 
              "let rec f (x : int) : int = if f 0 then x else x + 1;;",
              false);
         ]

let all_recur = "recursion" >::: [ rec_let_expr; recursion ]
