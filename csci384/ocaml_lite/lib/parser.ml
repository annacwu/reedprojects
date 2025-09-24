(** This module is responsible for parsing OCaml-lite code. *)

open Lexer
open Ast

(*************
 * Utilities *
 *************)

exception ParseError of string
(** A [ParseError] is raised when parsing fails, i.e., when the source code has
    a syntax error. *)

(** Throw an error when the input stream ended in the middle of a parsing rule.
    This is a utility function to avoid having to repeat the error string each
    time we encounter an unexpected end of input. Note that this function takes
    a dummy argument, so in order to throw an error, it needs to be called as
    [unexpected_end ()]. *)
let unexpected_end (_ : unit) : 'a =
  raise (ParseError "Unexpected end of input")

(** [expect t src] throws an exception whenever the first character of [src] is
    not [t]. If the first token in [src] is [t], then [t] is consumed and the
    tail of [src] is returned. *)
let expect (exp : token) : token list -> token list = function
  | [] -> unexpected_end ()
  | t :: ts ->
      if t = exp then ts
      else
        raise
          (ParseError
             ("Expected token: " ^ tok_to_str exp ^ " got: " ^ tok_to_str t))

(***************
 * Expressions *
 ***************)

  let rec parse_expr (source: token list) : expr * token list =
    let rec help acc src = match src with
      | Plus :: rest -> 
        let (t, r) = parse_term rest in
        help (EBinop(acc, BAdd, t)) r
      | Minus :: rest -> 
        let (t, r) = parse_term rest in 
        help (EBinop(acc, BSub, t)) r
      | ts -> (acc, ts) in let (t, r) = parse_term source in help t r

  and parse_term (source : token list) : expr * token list =
    let rec help acc src = match src with
      | Times :: rest ->
        let (t, r) = parse_factor rest in 
        help (EBinop(acc, BMul, t)) r 
      | Divide :: rest ->
        let (t, r) = parse_factor rest in 
        help (EBinop(acc, BDiv, t)) r  
      | Mod :: rest ->
        let (t, r) = parse_factor rest in 
        help (EBinop(acc, BMod, t)) r 
      | ts -> (acc, ts) in let (t, r) = parse_factor source in help t r

  and parse_factor (source: token list) : expr * token list = match source with
      | Negate :: rest -> 
        let (t, r) = parse_parens rest in
        (EUnop(UNegate, t), r)
      | _ -> parse_parens source

  and parse_parens (source: token list) : expr * token list = match source with
    | LParen :: rest ->
      let (t, r) = parse_expr rest in 
      (match r with
        | RParen :: rest -> (t, rest)
        | _ -> raise (ParseError "Expected )"))
    | Int i :: rest -> (EConst(CInt(i)), rest)
    | _ -> raise (ParseError "Expected <int> or (")

  let expr (source: token list) : expr * token list = 
    match parse_expr source with
    | (expr, []) -> (expr, source)
    | _ -> raise (ParseError "Expected end-of-input")
    



(*********
 * Types *
 *********)

let typ (_src : token list) : typ * token list = failwith "FILL IN in part 2"

(**********************
 * Top-level bindings *
 **********************)

let binding (_src : token list) : binding * token list =
  failwith "FILL IN in part 3"

(* For now our program parses source code that has exactly the form
   "let _ = print_string (string_of_int (<expr>)) ;;"
   This form will remain a valid OCaml-lite program throughout the entire
   semester, so testing can be more consistent if we start with this form.
   However, we don't want to deal with let bindings or function application
   yet, so we've just built the parser to expect this form for now. *)

(** [program src] parses a program from the given token list. *)
let program (src : token list) : program * token list =
  let r1 = expect Let src in
  let r2 = expect (Id "_") r1 in
  let r3 = expect Eq r2 in
  let r4 = expect (Id "print_string") r3 in
  let r5 = expect LParen r4 in
  let r6 = expect (Id "string_of_int") r5 in
  let r7 = expect LParen r6 in
  let expr, r8 = expr r7 in
  let r9 = expect RParen r8 in
  let r10 = expect RParen r9 in
  let r11 = expect DoubleSemicolon r10 in
  ( [
      BLet
        ( "_",
          [],
          None,
          EApp (EVar "print_string", EApp (EVar "string_of_int", expr)) );
    ],
    r11 )

(*************
 * Main code *
 *************)

(** [parse src] parses a program from [src] and ensures that it has consumed all
    tokens from [src]. *)
let parse (src : string) : program =
  match program (tokenize src) with
  | p, [] -> p
  | _, t :: _ ->
      raise (ParseError ("Expected end of input but got: " ^ tok_to_str t))

(** [parse_enter f src] parses the string [src] but begins from the parsing
    function [f]. This is a utility to make it easier to test subsections of the
    parser. *)
let parse_enter (f : token list -> 'a) (src : string) : 'a = f (tokenize src)
