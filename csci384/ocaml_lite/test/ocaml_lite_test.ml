open OUnit2
open Ocaml_lite.Lexer

let lex_tests =
  "test suite for tokenize"
  >::: [
         ( "random code" >:: fun _ ->
           assert_equal
             [
               Let;
               Id "f";
               LParen;
               Id "x";
               Colon;
               TInt;
               RParen;
               Colon;
               TString;
               Eq;
               If;
               Id "x";
               Lt;
               Int 0;
               Then;
               String "neg";
               Else;
               String "pos";
             ]
             (tokenize
                "let f (x : int) : string = if x < 0 then \"neg\" else \"pos\"")
         );
         ( "all tokens" >:: fun _ ->
           assert_equal
             [
               Let;
               Rec;
               If;
               Then;
               Else;
               Fun;
               True;
               False;
               Mod;
               TInt;
               TBool;
               TString;
               TUnit;
               Eq;
               Plus;
               Minus;
               Times;
               Divide;
               Lt;
               Concat;
               And;
               Or;
               Not;
               Negate;
               DoubleSemicolon;
               Colon;
               Arrow;
               LParen;
               RParen;
               Id "function_name";
               Int 32;
               String "str";
               Pipe;
               DoubleArrow;
               Tid "a";
             ]
             (tokenize
                ("let rec if then else fun true false mod int bool string "
               ^ "unit =+-*/<^ && || not ~;;: -> () function_name 32 "
               ^ "\"str\"|=>'a")) );
         ( "underscore id" >:: fun _ ->
           assert_equal [ Id "_x32" ] (tokenize "_x32") );
         ( "number id" >:: fun _ ->
           assert_equal [ Int 32; Id "xyz" ] (tokenize "32xyz") );
       ]

let _ = Printf.printf "Testing\n"
let _ = run_test_tt_main lex_tests
