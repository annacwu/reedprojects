open OUnit2
open Ocaml_lite_test_utils.Utils

let types =
  "types"
  >::: List.map make_type_ast_str
         [ ("tuple", Tup([Int; Bool])
        , "int * bool");
        ("parentheses tuple", Tup([Tup[Int; Bool]; Unit]), "(int * bool) * unit");
        ("custom type", Custom("my_type"), "my_type");
        ("nested tuple", Tup([Int; Tup([Bool; String])]), "int * (bool * string)") ]

let tuple_tests =
  "tuples"
  >::: List.map make_bind_ast_str
  [
    ("typed tuple", BLet("x", [], Some (Tup([Int; String; Unit])), ETup([EConst(CInt(1)); EConst(CString("test")); EConst(CUnit)])), "let x : int * string * unit = (1, \"test\", ())");
    ("untyped tuple", BLet("x", [], None, ETup([EConst(CInt(1)); EConst(CString("test")); EConst(CUnit)])), "let x = (1, \"test\", ())");
    ("nested tuple left", BLet("x", [], Some (Tup([Tup[Int; String]; Unit])), ETup([ETup([EConst(CInt(1)); EConst(CString("test"))]); EConst(CUnit)])), "let x : (int * string) * unit = ((1, \"test\"), ())");
    ("nested tuple right", BLet("y", [], Some (Tup([Bool; Tup([Int; String])])), ETup([EConst(CBool(false)); ETup([EConst(CInt(5)); EConst(CString("hello"))])])), "let y : bool * (int * string) = (false, (5, \"hello\"))")
  ]

let matching_tests =
  "matching"
  >::: List.map make_expr_ast_str
  [
    ("basic pattern", EMatch(EVar("lst"), [("Nil", [], EConst(CInt(0))); ("Cons", ["head"; "tail"], EBinop(EVar("head"), BAdd, EApp(EVar ("sum"), EVar ("tail"))))]), "match lst with | Nil => 0 | Cons (head, tail) => head + sum tail");
    ("another pattern", EMatch(EVar("src"), [("Var", ["x"], EMatch(EVar("x"), [("Id", ["x"], EVar("x")); ("_", [], EConst(CString("none")))])); ("_", [], EConst(CString("test")))]), "match src with | Var x => (match x with | Id x => x | _ => \"none\") | _ => \"test\"");
    ("multiple args pattern", EMatch(EVar("opt"), [("Some", ["a"; "b"; "c"], ETup([EVar("a"); EVar("b"); EVar("c")])); ("None", [], EConst(CUnit))]), "match opt with | Some (a, b, c) => (a, b, c) | None => ()");
  ]


let expr_tests = "expressions" >::: [ types; tuple_tests; matching_tests ]

let top_level_let =
  "top-level lets"
  >::: List.map make_bind_ast_str
         [
           ( "basic adt",
             ADT("int_list", [("Nil", None); ("Cons", Some (Tup([Int; Custom("int_list")])))]),
             "type int_list = | Nil | Cons of int * int_list ;;" );
           ( "option adt",
             ADT("option", [("None", None); ("Some", Some Int)]),
             "type option = | None | Some of int ;;" );
           ( "tree adt",
             ADT("tree", [("Leaf", Some Int); ("Node", Some (Tup([Custom("tree"); Int; Custom("tree")])))]),
             "type tree = | Leaf of int | Node of tree * int * tree ;;" );
         ]
let binding_tests = "bindings" >::: [ top_level_let ]

let parse_tests : test = "parser tests" >::: [expr_tests; binding_tests]