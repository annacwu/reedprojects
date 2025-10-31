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
(*assert_failure "typechecking failed with unexpected error")*)

let let_expr =
  "let"
  >::: List.map tc_program
         [
           ( "pass function",
             "let x : int -> int = let f (y : int) = y in f;;",
             true );
           ( "fail function",
             "let x : int -> int = let f (y : unit) = y in f;;",
             false );
         ]

let rec_let_expr =
  "let rec"
  >::: List.map tc_program
         [
           ( "let rec pass",
             "let _ = let rec f x = if x < 0 then x else f (x - 1) in f 3;;",
             true );
           ( "let rec fail",
             "let rec f x = if f (x - 1) < 0 then true else false;;",
             false );
         ]

let lambda_expr =
  "lambda"
  >::: List.map tc_program
         [
           ("pass no type", "let x : int -> int = fun y => y + 1;;", true);
           ("pass with type", "let x : int -> int = fun (y : int) => y;;", true);
         ]

let expressions = "expressions" >::: [ let_expr; lambda_expr ]

let functions =
  "function definitions"
  >::: List.map tc_program
         [ ("id pass", "let f (x : int) : int = x;; let _ = f 2;;", true) ]

let all_functions = "functions" >::: [ expressions; functions ]

let polymorphism =
  "polymorphism"
  >::: List.map tc_program
         [
           ("id pass", "let f x = x;; let _ = f 2;; let _ = f ();;", true);
           ("id fail", "let _ = (fun f => (f 2, f ())) (fun x => x);;", false);
           ("id in expr", "let _ = let id x = x in (id 2, id ());;", true);
         ]

let recursion =
  "recursive definitions"
  >::: List.map tc_program
         [
           ( "bad self type",
             "let rec f (x : int) = if x < 1 then f x else f true;;",
             false );
         ]

let all_recur = "recursion" >::: [ rec_let_expr; recursion ]
