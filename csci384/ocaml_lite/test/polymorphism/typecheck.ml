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
           ("pass function", "let x = let f y = y in f;;", true); 
           ("fail function", "let f = let f x = x + 1 in f ();;", false);
           ( "curried type", 
             "let add x y = x + y;; let f = add;;",
             true);
          ( "lyell's test",
            "let x = let f y z = if z then y else y + 1 in f;;",
            true
          );
          ("greg fail test",
          "let x : int -> int = fun y => if y then 1 else 2;;",
          false);
          ("well-typed", 
          "let f x = x ;;
          let _ = f 2 ;;
          let _ = f () ;;", 
          true);
          ("not well-typed", "let z = (fun f => let _ = f 2 in f ()) (fun x => x);;", false);
          ("big example", "let z = let maybe f x y = if x then f y else y in maybe (fun x => x + 1) true 1;;", true)
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
  >::: List.map tc_program [ 
    ("pass with type", "let x = fun y => y;;", true) ;
    ("simple lambda", 
            "let f = fun x => x + 1;;", 
            true);
    ]


let expressions = "expressions" >::: [ let_expr; lambda_expr ]

let functions =
  "function definitions"
  >::: List.map tc_program [ 
    ("id pass", "let f x = x;; let _ = f 2;;", true);
        ("param type error", "let inc (x : int) = x && true;;", false);
          ("two params", "let add x y = x + y;;", true);
          ("two params type error", "let add x (y : bool) = x + y;;", false); 
          ("greg test", "let f x y = x (x y);; let _ = f (fun x => x + 1) 0;;", true);
     ]

let all_functions = "functions" >::: [ expressions; functions ]

let polymorphism =
  "polymorphism"
  >::: List.map tc_program
         [
           ("id pass", "let f x = x;; let _ = f 2;; let _ = f ();;", true);
           ("id fail", "let _ = (fun f => (f 2, f ())) (fun x => x);;", false);
           ("id in expr", "let _ = let id x = x in (id 2, id ());;", true);
           ("complex expression", "let _ = let twice f x = f (f x) in let id x = x in fun y => if y then twice id 0 else 2;;", true);
         ]

let recursion =
  "recursive definitions"
  >::: List.map tc_program
         [
           ( "bad self type",
             "let rec f x = if x < 1 then f x else f true;;",
             false );
          ( "good self type",
             "let rec f x = if x < 1 then x else f (x - 1);;",
             true );
            ( "bad type instructions example", 
              "let rec f x = if f 0 then x else x + 1;;",
              false);
         ]

let all_recur = "recursion" >::: [ rec_let_expr; recursion ]
let top_level = "all tests" >::: [ all_functions; polymorphism; all_recur ]
