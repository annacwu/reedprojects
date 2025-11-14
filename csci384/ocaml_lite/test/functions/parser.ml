open OUnit2
open Ocaml_lite_test_utils.Utils
open Ocaml_lite.Ast
open Ocaml_lite.Parser

let associativity_tests =
  "associativity"
  >::: List.map make_expr_str_str [ ("app", "(x y) z", "x y z") ]

let recursion_tests = 
  "recursion"
  >::: List.map make_expr_ast_str
    [(
      "expression let no types",
      ELetRec("f", [Param("x", None); Param("m", None)], None, (ECond((EBinop(EVar "x", BLt, EConst(CInt 0))), EVar "m", (EApp((EApp(EVar "f", EBinop(EVar "x", BSub, EConst(CInt 1)))), EBinop(EVar "m", BAdd, EConst(CInt 1)))))), EApp(EApp(EVar "f", EConst(CInt 2)), EConst(CInt 0))),
      "let rec f x m = if x < 0 then m else f (x - 1) (m + 1) in f 2 0"
    )]

let expr_tests = "expressions" >::: [ associativity_tests; recursion_tests ]

let types =
  "types"
  >::: List.map make_type_ast_str
         [
           ("function", Func(Int, Bool), "int -> bool");
           ("function right assoc", Func(Int, Func(Int, Bool)), "int -> int -> bool");
           ("function with parens", Func(Func(Int, Int), Bool), "(int -> int) -> bool");
           ("super nested function type", Func(Int, Func(Int, Func(Int, Int))), "int -> int -> int -> int");
         ]

let typed_exprs =
  "expressions"
  >::: List.map make_expr_ast_str
         [
           ( "let with args",
             ELet("f", [Param("x", Some Int); Param("y", Some Bool)], Some Int, EVar "x", EApp(EApp(EVar "f", EConst(CInt(2))), EConst(CBool(false)))),
             "let f (x : int) (y : bool) : int = x in f 2 false" );
          (
            "rec let",
            ELetRec("f", [Param("x", Some Int); Param("m", Some Int)], Some Int, (ECond((EBinop(EVar "x", BLt, EConst(CInt 0))), EVar "m", (EApp((EApp(EVar "f", EBinop(EVar "x", BSub, EConst(CInt 1)))), EBinop(EVar "m", BAdd, EConst(CInt 1)))))), EApp(EApp(EVar "f", EConst(CInt 2)), EConst(CInt 0))),
            "let rec f (x : int) (m : int) : int = if x < 0 then m else f (x - 1) (m + 1) in f 2 0"
          )
         ]
       @ List.map expr_expect_failure
           [ ("function missing arg", "let f : -> int = 2 in f") ]

let type_tests = "type suite" >::: [ types; typed_exprs ]

let top_level_let =
  "top-level lets"
  >::: List.map make_bind_ast_str
         [
           ( "let with params and type",
             BLet("f", [Param("x", Some Int)], Some Int, EBinop(EVar "x", BAdd, EConst(CInt(1)))),
             "let f (x : int) : int = x + 1" );
            ("let with fun", 
              BLet("f", [], None, EAnon([Param("x", Some Int)], Some Int, EBinop(EVar "x", BAdd, EConst(CInt 1)))),
              "let f = fun (x : int) : int => x + 1"
            )
         ]

let binding_tests = "bindings" >::: [ top_level_let ]

let program_tests =
  "top-level programs"
  >::: [
         ( "fake factorial" >:: fun _ ->
           assert_equal ~printer:program_to_str
             ([
              BLet("f", [Param("x", Some Int)], Some String, ECond((EBinop(EVar "x", BLt, EConst(CInt 0))),EConst(CString "neg"), EConst(CString "pos") )); 
              BLet("fact", [Param("x", Some Int)], Some Int, ECond((EBinop(EVar "x", BEq, EConst(CInt 1))),EVar "x", EBinop(EVar "x", BMul, EBinop(EVar "x", BSub, EConst(CInt 1)))));
              BLet("_", [], None, EApp(EVar "f", EApp(EVar "fact", EConst(CInt 2))))
              ])
             (parse
                ("let f (x : int) : string = if x < 0 then \"neg\" else \
                  \"pos\";;"
               ^ "let fact (x : int) : int = if x = 1 then x else x * (x - 1);;"
               ^ "let _ = f (fact 2);;")) );
       ]

let parse_tests =
  "test suite for parsing"
  >::: [ expr_tests; type_tests; binding_tests; program_tests ]
