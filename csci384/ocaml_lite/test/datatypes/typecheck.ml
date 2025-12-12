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
  | TypeError s -> assert_bool ("program failed typechecking with typeerror: " ^ s) (not pass)
  | OUnitTest.OUnit_failure _ as e -> raise e
  | e -> raise e

let tuple_tests =
  "let"
  >::: List.map tc_program
         [
            ("pass tuple", "let x : int * string * unit = (1, \"test\", ());;", true); 
            ("fail tuple", "let x : int * string * unit = (1, \"test\", 4);;", false);
            ("not long enough tuple", "let x : int * string * unit = (1, \"test\");;", false);
            ("polymorphic tuple", "let x = (1, \"test\", ());;", true);
            ("id fail", "let _ = (fun f => (f 2, f ())) (fun x => x);;", false);
            ("id in expr", "let _ = let id x = x in (id 2, id ());;", true);
         ]

let adt_tests =
  "adts"
  >::: List.map tc_program
         [
            ("basic adt",
             "type int_list = | Nil | Cons of int * int_list ;;", true );
            ("bad adt",
             "type int_list = | Nil | Cons of int * test_undefined_type ;;", false );
            ("adt in use", "type int_list = | Nil | Cons of int * int_list ;; let x = Cons(1, Nil) ;;", true);
            ("bad adt in use", "type int_list = | Nil | Cons of int * int_list ;; let x = Cons(true, Nil) ;;", false); 
            ("nested adt in use", "type int_list = | Nil | Cons of int * int_list ;; let x = Cons(1, Cons(2, Nil)) ;;", true);  
            ("adt with function", "type int_list = | Nil | Cons of int * int_list ;; let f x = Cons(x, Nil) ;; let _ = f 2 ;;", true); 
             ("bad adt with function", "type int_list = | Nil | Cons of int * int_list ;; let f x = Cons(x, Nil) ;; let _ = f true ;;", false); 
         ]

let match_tests =
  "adts"
  >::: List.map tc_program
         [
           ("basic pattern", 
           "type int_list = | Nil | Cons of int * int_list ;; 
            let f lst = match lst with | Nil => 0 | Cons (head, tail) => head + 1 ;;", true);
           ("missing variable", 
           "type int_list = | Nil | Cons of int * int_list ;; 
           let f lst = match lst with | Nil => 0 | Cons (head) => head + sum tail ;;", false);
           ("wrong constructor type", 
           "type int_list = | Nil | Cons of int * int_list ;; 
            type option = | None | Some of int ;; 
           let f lst = match lst with | Nil => 0 | Some x => x ;;", false);
           ("not matching bodies", 
           "type option = | None | Some of int ;; 
            let f opt = match opt with | None => false | Some x => x ;;", false);
          ("recursive pattern", 
           "type int_list = | Nil | Cons of int * int_list ;; 
            let rec f lst = match lst with | Nil => 0 | Cons (head, tail) => head + f (tail) ;;", true);
          ("double definition",
          "type one = | Nil ;;
          type two = | Nil ;;
          let x : one = Nil ;;", true);
          ("empty match", 
           "type int_list = | Nil | Cons of int * int_list ;; 
            let f lst = match lst with | Cons (head, tail) => head + 1 | _ => 0 ;;", true);
         ]

let tc_tests : test = "typecheck tests" >::: [tuple_tests; adt_tests; match_tests]
