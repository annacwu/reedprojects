open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Interpret

let pattern_matching =
  "pattern matching"
  >::: List.map program_test
         [
           ( "basic pattern match",
             check_val "res" (VInt 2),
             "type int_list = | Nil | Cons of int * int_list ;;
             let lst = Cons(1, Nil);;
             let res = match lst with
             | Nil => 0
             | Cons(head, tail) => head + 1 ;;",
             "Couldn't match on basic constructor" );
           ( "match nil case",
             check_val "res" (VInt 0),
             "type int_list = | Nil | Cons of int * int_list ;;
             let lst = Nil;;
             let res = match lst with
             | Nil => 0
             | Cons(head, tail) => head ;;",
             "Couldn't match Nil case" ); 
            ( "nested pattern",
             check_val "res" (VInt 15),
             "type option = | None | Some of int ;;
             type int_list = | Nil | Cons of int * int_list ;;
             let opt = Some(10);;
             let lst = Cons(5, Nil);;
             let res = match opt with
             | None => 0
             | Some(x) => (match lst with 
                | Nil => 0
                | Cons(head, tail) => head + x) ;;",
             "Couldn't use multiple type definitions" ); 
            ( "match with bool expressions",
             check_val "res" (VBool true),
             "type option = | None | Some of int ;;
             let x = Some(5);;
             let res = match x with
             | None => false
             | Some(v) => 0 < v ;;",
             "Couldn't match with bool bodies" );
            ( "three constructor type",
             check_val "color" (VString "yellow"),
             "type traffic_light = | Red | Yellow | Green ;;
             let light = Yellow;;
             let color = match light with
             | Red => \"red\"
             | Yellow => \"yellow\"
             | Green => \"green\" ;;",
             "Couldn't match three-way constructor" );
            ("apply function",
            check_val "res" (VInt 1),
            "type int_list = | Nil | Cons of int * int_list ;;
             let lst = Cons(1, Nil);;
             let rec sum l = match l with
             | Nil => 0
             | Cons(head, tail) => head + sum tail ;;
             let res = sum lst ;;",
             "Couldn't apply function in body");
            ("nested",
            check_val "res" (VInt 6),
            "type int_list = | Nil | Cons of int * int_list ;;
             let lst = Cons(1, Cons(2, Cons(3, Nil)));;
             let rec sum l = match l with
             | Nil => 0
             | Cons(head, tail) => head + sum tail ;;
             let res = sum lst ;;",
             "Couldn't apply function in body");
            ("empty match", 
            check_val "res" (VInt 1),
            "type int_list = | Nil | Cons of int * int_list ;;
             let lst = Nil;;
             let res = match lst with
             | Cons(head, tail) => head
             | _ => 1 ;;", "couldn't match with _");
         ]


let interp_tests : test = "interpreter tests" >::: [ pattern_matching; ]
