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
 let rec parse_bind (source: token list) : expr * token list = 
  match source with
    | Let :: Id x :: Eq :: rest -> let (e1, r1) = parse_if rest in 
    (* let _ = print_endline("in the first case") in *)
    (match r1 with 
      | In :: rest -> let (e2, r2) = parse_if rest in 
      (ELet(x, [], None, e1, e2), r2)
      | _ -> raise (ParseError ("Expected in")))
    | Let :: Id x :: Colon :: t :: Eq :: rest -> let (e1, r1) = parse_if rest in 
    (* let _ = print_endline("in the second case") in *)
    (match r1 with 
      | In :: rest -> let (e2, r2) = parse_if rest in (match t with
        | TInt -> (ELet(x, [], Some Int, e1, e2), r2)
        | TBool -> (ELet(x, [], Some Bool, e1, e2), r2)
        | TUnit -> (ELet(x, [], Some Unit, e1, e2), r2)
        | TString -> (ELet(x, [], Some String, e1, e2), r2)
        | _ -> raise (ParseError ("Expected type"))
      )
      | _ -> raise (ParseError ("Expected in")))
    | Let :: _ -> raise (ParseError ("Unexpected let binding structure"))
    | _ -> parse_if source

 and parse_if (source: token list) : expr * token list = match source with
    | If :: rest -> 
      let (cond, r1) = parse_or rest in 
      (match r1 with
        | Then :: rest -> 
          let (body, r2) = parse_or rest in 
          (match r2 with
            | Else :: rest -> let (ebody, r3) = parse_or rest in 
            (ECond(cond, body, ebody), r3)
            | _ -> raise (ParseError "Expected else"))
        | _ -> raise (ParseError "Expected then"))
    | _ -> parse_or source

  and parse_or (source: token list) : expr * token list = 
    let rec help acc src = match src with
      | Or :: rest -> 
        let (t, r) = parse_and rest in 
        help (EBinop(acc, BOr, t)) r
      | ts -> (acc, ts) in let (t, r) = parse_and source in help t r
  
  and parse_and (source: token list) : expr * token list = 
    let rec help acc src = match src with
      | And :: rest -> 
        let (t, r) = parse_comp rest in 
        help (EBinop(acc, BAnd, t)) r
      | ts -> (acc, ts) in let (t, r) = parse_comp source in help t r
 
  and parse_comp (source: token list) : expr * token list = 
    let (e1, src) = parse_concat source in
    match src with
    | Lt :: rest -> 
      let (e2, r) = parse_concat rest in 
      (EBinop (e1, BLt, e2), r)
    | Eq :: rest ->
      let (e2, r) = parse_concat rest in 
      (EBinop(e1, BEq, e2), r) 
    | _ -> parse_concat source 

    and parse_concat (source: token list) : expr * token list =
    let rec help acc src = match src with
      | Concat :: rest ->
        let (t, r) = parse_expr rest in 
        help (EBinop(acc, BCat, t)) r 
      | ts -> (acc, ts) in let (t, r) = parse_expr source in help t r 

  and parse_expr (source: token list) : expr * token list =
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
        let (t, r) = parse_unop rest in 
        help (EBinop(acc, BMul, t)) r 
      | Divide :: rest ->
        let (t, r) = parse_unop rest in 
        help (EBinop(acc, BDiv, t)) r  
      | Mod :: rest ->
        let (t, r) = parse_unop rest in 
        help (EBinop(acc, BMod, t)) r 
      | ts -> (acc, ts) in let (t, r) = parse_unop source in help t r

  and parse_unop (source: token list) : expr * token list = match source with
    | Negate :: rest -> 
      let (t, r) = parse_app rest in
      (EUnop(UNegate, t), r)
    | Not :: rest -> 
      let (t, r) = parse_app rest in
      (EUnop(UNot, t), r)
    | _ -> parse_app source

  and parse_app (source: token list) : expr * token list = 
    let rec help acc src = match src with
      | (Id _ | Int _ | True | False | LParen | If | Let) :: _ -> 
        let (arg, r) = parse_factor src in
        help (EApp(acc, arg)) r
      | _ -> (acc, src)
      in let (func, r1) = parse_factor source in help func r1
  
  and parse_factor (source: token list) : expr * token list = match source with
    | LParen :: rest -> (
        match rest with
        | RParen :: rest -> (EConst(CUnit), rest)
        | _ ->
            let (t, r) = parse_bind rest in
            match r with
            | RParen :: rest -> (t, rest)
            | _ -> raise (ParseError "Expected )")
      )
    | Int i :: rest -> (EConst(CInt(i)), rest)
    | True :: rest -> (EConst(CBool(true)), rest)
    | False :: rest -> (EConst(CBool(false)), rest)
    | String s :: rest -> (EConst(CString(s)), rest)
    | Id x :: rest -> (EVar x, rest)
    | If :: _ -> parse_if source
    | Let :: _ -> parse_bind source
    | _ -> raise (ParseError ("Unexpected expression: " ^ (String.concat " " (List.map tok_to_str source))))

  let expr (source: token list) : expr * token list = 
    parse_bind source
    
    
(*********
 * Types *
 *********)

  let rec typ_parse_bind (source: token list) : typ * token list = match source with
    | Let :: Id _ :: Eq :: rest -> let (e1, r1) = typ_parse_if rest in (match r1 with 
      | In :: rest -> let (_, r2) = typ_parse_if rest in 
      (e1, r2)
      | _ -> raise (ParseError ("Expected in")))
    | Let :: Id _ :: Colon :: t :: Eq :: rest -> let (_, r1) = typ_parse_if rest in (match r1 with 
      | In :: rest -> let (_, r2) = typ_parse_if rest in (match t with
        | TInt -> (Int, r2)
        | TBool -> (Bool, r2)
        | TUnit -> (Unit, r2)
        | TString -> (String, r2)
        | _ -> raise (ParseError ("Expected type"))
      )
      | _ -> raise (ParseError ("Expected in")))
    | _ -> typ_parse_if source

  and typ_parse_if (source: token list) : typ * token list = match source with
    | If :: rest -> 
      let (_, r1) = typ_parse_or rest in 
      (match r1 with
        | Then :: rest -> 
          let (body, r2) = typ_parse_or rest in 
          (match r2 with
            | Else :: rest -> let (_, r3) = typ_parse_or rest in 
            (body, r3)
            | _ -> raise (ParseError "Expected else"))
        | _ -> raise (ParseError "Expected then"))
    | _ -> typ_parse_or source

  and typ_parse_or (source: token list) : typ * token list = 
    let rec help acc src = match src with
      | Or :: rest -> 
        let (_, r) = typ_parse_and rest in 
        help Bool r
      | ts -> (acc, ts) in let (t, r) = typ_parse_and source in help t r

  and typ_parse_and (source: token list) : typ * token list = 
    let rec help acc src = match src with
      | And :: rest -> 
        let (_, r) = typ_parse_comp rest in 
        help Int r
      | ts -> (acc, ts) in let (t, r) = typ_parse_comp source in help t r
 
  and typ_parse_comp (source: token list) : typ * token list = 
    let (_, src) = typ_parse_concat source in
    match src with
    | Lt :: rest -> 
      let (_, r) = typ_parse_concat rest in 
      (Bool, r)
    | Eq :: rest ->
      let (_, r) = typ_parse_concat rest in 
      (Bool, r) 
    | _ -> typ_parse_concat source 

    and typ_parse_concat (source: token list) : typ * token list =
    let rec help acc src = match src with
      | Concat :: rest ->
        let (_, r) = typ_parse_expr rest in 
        help String r 
      | ts -> (acc, ts) in let (t, r) = typ_parse_expr source in help t r 

  and typ_parse_expr (source: token list) : typ * token list =
    let rec help acc src = match src with
      | Plus :: rest -> 
        let (_, r) = typ_parse_term rest in
        help Int r
      | Minus :: rest -> 
        let (_, r) = typ_parse_term rest in 
        help Int r
      | ts -> (acc, ts) in let (t, r) = typ_parse_term source in help t r

  and typ_parse_term (source : token list) : typ * token list =
    let rec help acc src = match src with
      | Times :: rest ->
        let (_, r) = typ_parse_factor rest in 
        help Int r 
      | Divide :: rest ->
        let (_, r) = typ_parse_factor rest in 
        help Int r  
      | Mod :: rest ->
        let (_, r) = typ_parse_factor rest in 
        help Int r 
      | ts -> (acc, ts) in let (t, r) = typ_parse_factor source in help t r

  and typ_parse_factor (source: token list) : typ * token list = match source with
    | Negate :: rest -> 
      let (t, r) = typ_parse_factor rest in
      (t, r)
    | Not :: rest -> 
      let (t, r) = typ_parse_factor rest in
      (t, r)
    | LParen :: rest -> (
        match rest with
        | RParen :: rest -> (Unit, rest)
        | _ ->
            let (t, r) = typ_parse_if rest in
            match r with
            | RParen :: rest -> (t, rest)
            | _ -> raise (ParseError "Expected )")
      )
    | Int _ :: rest -> (Int, rest)
    | True :: rest -> (Bool, rest)
    | False :: rest -> (Bool, rest)
    | String _ :: rest -> (String, rest)
    | TInt :: rest -> (Int, rest)
    | TBool :: rest -> (Bool, rest)
    | TString :: rest -> (String, rest)
    | TUnit :: rest -> (Unit, rest)
    | Id _ :: rest -> (String, rest)
    | If :: _ -> typ_parse_if source
    | Let :: _ -> typ_parse_bind source
    | _ -> raise (ParseError ("Unexpected expression: " ^ (String.concat " " (List.map tok_to_str source))))

let typ (_src : token list) : typ * token list = 
  typ_parse_bind _src

(**********************
 * Top-level bindings *
 **********************)

let binding (_src : token list) : binding * token list = match _src with
  | Let :: Id x :: Eq :: rest -> let (e1, r1) = expr rest in 
      (* let _ = print_endline("\n in the binding") in *)
      (BLet(x, [], None, e1), r1)
  | Let :: Id x :: Colon :: t :: Eq :: rest -> let (e1, r1) = expr rest in (match t with
        | TInt -> (BLet(x, [], Some Int, e1), r1)
        | TBool -> (BLet(x, [], Some Bool, e1), r1)
        | TUnit -> (BLet(x, [], Some Unit, e1), r1)
        | TString -> (BLet(x, [], Some String, e1), r1)
        | _ -> raise (ParseError ("Expected type in upper level binding"))
      )
  | _ -> raise (ParseError("Unexpected expression: " ^ (String.concat " " (List.map tok_to_str _src))))

(** [program src] parses a program from the given token list. *)
let rec program (src: token list) (pr: binding list): program * token list =
  match src with 
  | Let :: _ -> 
    let inst, r1 = binding src in
    let r2 = expect DoubleSemicolon r1 in
    let new_pr = inst :: pr in 
    let res = if r2 = [] then (new_pr, r2) else program r2 new_pr in res
  | _ -> raise (ParseError ("Unexpected program structure: " ^ (String.concat " " (List.map tok_to_str src))))
  

(* For now our program parses source code that has exactly the form
   "let _ = print_string (string_of_int (<expr>)) ;;"
   This form will remain a valid OCaml-lite program throughout the entire
   semester, so testing can be more consistent if we start with this form.
   However, we don't want to deal with let bindings or function application
   yet, so we've just built the parser to expect this form for now. *)

(* let program (src : token list) : program * token list =
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
    r11 ) *)

(*************
 * Main code *
 *************)

(** [parse src] parses a program from [src] and ensures that it has consumed all
    tokens from [src]. *)
let parse (src : string) : program =
  match program (tokenize src) [] with
  | p, [] -> p
  | _, t :: _ ->
      raise (ParseError ("Expected end of input but got: " ^ tok_to_str t))

(** [parse_enter f src] parses the string [src] but begins from the parsing
    function [f]. This is a utility to make it easier to test subsections of the
    parser. *)
let parse_enter (f : token list -> 'a) (src : string) : 'a = f (tokenize src)
