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
            
(*********
 * Types *
 *********)

(* helper for all parsing functions to parse types *)
  let rec parse_typ (source: token list) : typ * token list = 
    let t1, r = parse_base_typ source in match r with
      | Arrow :: r2 ->
          let t2, r3 = parse_typ r2 in
          (Func(t1, t2), r3)
      | Times :: _ -> 
          let l, r3 = parse_tup_typ t1 r in 
          (l, r3)
      | _ -> (t1, r)

    and parse_tup_typ (first: typ) (source: token list) : typ * token list =
    let rec help acc src = match src with
      | Times :: rest ->
        let t, r2 = parse_base_typ rest in
        let next = List.append acc [t] in
        help next r2
      | _ -> (Tup(acc), src)
      in help [first] source

  and parse_base_typ (source: token list) : typ * token list =
    match source with
    | TInt :: rest -> (Int, rest)
    | TBool :: rest -> (Bool, rest)
    | TUnit :: rest -> (Unit, rest)
    | TString :: rest -> (String, rest)
    | Id t :: rest -> (Custom t, rest)
    | LParen :: rest ->
        let t, r1 = parse_typ rest in
        (match r1 with
         | RParen :: r2 -> (t, r2) 
         | _ -> raise (ParseError "Expected )"))
    | _ -> raise (ParseError ("Expected type, got " ^ (String.concat " " (List.map tok_to_str source))))
  
  let typ (_src : token list) : typ * token list = 
  parse_typ _src

(* helper for all parsing functions to parse parameters *)
 let rec parse_params (source: token list) (ps: params) : params * token list = match source with
      | LParen :: Id x :: Colon :: t  -> 
        let typ, r1 = parse_typ t in
        let p = List.append ps [Param(x, Some typ)] in (match r1 with
         | RParen :: r2 -> parse_params r2 p
         | _ -> raise (ParseError "Expected )")) 
      | Id x :: rest -> 
        let p = List.append ps [Param(x, None)] in 
        parse_params rest p
      | Colon :: _ -> ps, source
      | Eq :: _ -> ps, source
      | DoubleArrow :: _ -> ps, source
      | _ -> raise (ParseError ("Unexpected parameters structure: "^  (String.concat " " (List.map tok_to_str source)) ))  


(***************
 * Expressions *
 ***************)
 
 let rec parse_bind (source: token list) : expr * token list = match source with
    | Let :: Id x :: rest -> 
      let (ps, r1) = parse_params rest [] in (match r1 with
        | Eq :: rest -> let (e1, r2) = parse_fun rest in (match r2 with 
          | In :: rest -> 
            let (e2, r3) = parse_fun rest in 
            (ELet(x, ps, None, e1, e2), r3)
          | _ -> raise (ParseError ("Expected in")))
        | Colon :: rest -> 
            let typ, next = parse_typ rest in 
            let (e1, r2) = parse_fun next in (match r2 with 
              | In :: r3 -> 
                let (e2, r4) = parse_fun r3 in 
                (ELet(x, ps, Some typ, e1, e2), r4)
              | _ -> raise (ParseError ("Expected in")))
        | _ -> raise (ParseError ("Expected Eq or let type")))
    | Let :: Rec :: Id x :: rest -> 
      let (ps, r1) = parse_params rest [] in (match r1 with
        | Eq :: rest -> let (e1, r2) = parse_fun rest in (match r2 with 
          | In :: rest -> 
            let (e2, r3) = parse_fun rest in 
            (ELetRec(x, ps, None, e1, e2), r3)
          | _ -> raise (ParseError ("Expected in")))
        | Colon :: rest -> 
            let typ, next = parse_typ rest in 
            let (e1, r2) = parse_fun next in (match r2 with 
              | In :: r3 -> 
                let (e2, r4) = parse_fun r3 in 
                (ELetRec(x, ps, Some typ, e1, e2), r4)
              | _ -> raise (ParseError ("Expected in")))
        | _ -> raise (ParseError ("Expected Eq or let type"))) 
    | _ -> parse_fun source
  
  and parse_fun (source : token list) :  expr * token list = match source with
    | Fun :: rest -> 
        let ps, r1 = parse_params rest [] in (match r1 with 
          | Colon :: rest -> 
              let typ, r2 = parse_typ rest in 
              let e, r3 = parse_if r2 in 
              EAnon(ps, Some typ, e), r3
          | _ -> let e, r3 = parse_if r1 in 
              EAnon(ps, None, e), r3)
    | _ -> parse_match source
  
  and parse_match (source : token list) : expr * token list = match source with
      | Match :: rest -> 
        let e, r1 = parse_if rest in (match r1 with
          | With :: rest -> 
            (* let _ = print_endline("got e = " ^ expr_to_str e) in *)
            let p, r2 = parse_patterns [] rest in 
            EMatch(e, p), r2
          | _ -> raise( ParseError ("Expected with")
        ))
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
      | (Id _ | Int _ | True | False | String _ | LParen | If | Let) :: _ -> 
        let (arg, r) = parse_factor src in
        help (EApp(acc, arg)) r
      | _ -> (acc, src)
    in let (func, r1) = parse_factor source in help func r1

  and parse_tup (first: expr) (source: token list) : expr * token list =
    let rec help acc src = match src with
      | Comma :: rest ->
        let t, r2 = parse_bind rest in
        let next = List.append acc [t] in
        help next r2
      | _ -> (ETup(acc), src)
      in help [first] source

  and parse_factor (source: token list) : expr * token list = match source with
    | LParen :: rest -> (
        match rest with
        | RParen :: rest -> (EConst(CUnit), rest)
        | _ ->
            let (t1, r) = parse_bind rest in
             match r with
            | Comma :: _ ->
                let (tup, r2) = parse_tup t1 r in
                (match r2 with
                 | RParen :: rest -> (tup, rest)
                 | _ -> raise (ParseError "Expected ) after tuple"))
            | RParen :: rest -> (t1, rest)
            | _ -> raise (ParseError "Expected )")
      )
    | Int i :: rest -> (EConst(CInt(i)), rest)
    | True :: rest -> (EConst(CBool(true)), rest)
    | False :: rest -> (EConst(CBool(false)), rest)
    | String s :: rest -> (EConst(CString(s)), rest)
    | Id x :: rest -> (EVar x, rest)
    | If :: _ -> parse_if source
    | Let :: _ -> parse_bind source
    | Eq :: rest -> parse_bind rest
    | DoubleArrow :: rest -> parse_fun rest
    | Fun :: _ -> parse_fun source
    | Pipe :: _ -> parse_bind source
    | _ -> raise (ParseError ("Unexpected expression: " ^ (String.concat " " (List.map tok_to_str source))))

    (* helper for getting the list of patterns for an EMatch expression *)
  and parse_patterns (acc: pattern list) (src: token list) : (pattern list) * token list = let rec get_pattern_vars (acc: id list) (psrc : token list) : id list * token list = match psrc with 
      | Id x :: rest ->  get_pattern_vars (List.append acc [x]) rest
      | LParen :: rest -> 
        let vars, r = get_pattern_vars acc rest in (match r with
          | RParen :: r2 -> vars, r2
          | _ -> raise (ParseError "Expected ) in pattern")) 
      | Comma :: rest -> get_pattern_vars acc rest
      | _ -> acc, psrc 
  in match src with
  | Pipe :: Id x :: rest -> 
    (* let _ = print_endline("parsing this pattern: " ^ x) in *)
    let vars, r1 = get_pattern_vars [] rest in 
    (* let _ = print_endline("this patterns expression is: " ^ String.concat " " (List.map tok_to_str rest) ) in *)
    let e, r2 = parse_if r1 in 
    parse_patterns (List.append acc [(x, vars, e)]) r2
  | _ -> 
    (* let _ = print_endline("returning: " ^ String.concat " " (List.map tok_to_str src) ) in  *)
    acc, src

  let expr (source: token list) : expr * token list = 
    parse_bind source

(**********************
 * Top-level bindings *
 **********************)

(* helper for parsing ADTs in top-level bindings *)
let rec make_constructors (acc: (id * typ option) list) (src: token list) : (constructor list) * token list= match src with
  | Pipe :: Id x :: Of :: typ -> 
    let t, r = parse_typ typ in 
    make_constructors (List.append acc [(x, Some t)]) r
  | Pipe :: Id x :: rest -> make_constructors (List.append acc [(x, None)]) rest
  | _ -> acc, src
  
let binding (_src : token list) : binding * token list = match _src with
    | Let :: Id x :: rest -> 
      let (ps, r1) = parse_params rest [] in (match r1 with
        | Eq :: rest -> let (e1, r2) = parse_bind rest in (BLet(x, ps, None, e1), r2)
        | Colon :: rest -> 
            let typ, next = parse_typ rest in 
            let (e1, r2) = parse_bind next in (BLet(x, ps, Some typ, e1), r2)
        | _ -> raise (ParseError ("Expected type in upper level binding")))
    | Let :: Rec :: Id x :: rest -> 
      let (ps, r1) = parse_params rest [] in (match r1 with
        | Eq :: rest -> let (e1, r2) = parse_bind rest in (BLetRec(x, ps, None, e1), r2)
        | Colon :: rest -> 
            let typ, next = parse_typ rest in 
            let (e1, r2) = parse_bind next in (BLetRec(x, ps, Some typ, e1), r2)
        | _ -> raise (ParseError ("Expected type in upper level binding")))
    | Type :: Id x :: Eq :: rest -> 
      let constructors, r = make_constructors [] rest in
      ADT(x, constructors), r 
  | _ -> raise (ParseError("Unexpected expression: " ^ (String.concat " " (List.map tok_to_str _src))))

(** [program src] parses a program from the given token list. *)
let rec program (src: token list) (pr: binding list): program * token list =
  match src with 
  | Let :: _ -> 
    let inst, r1 = binding src in
    let r2 = expect DoubleSemicolon r1 in
    let new_pr = List.append pr [inst] in 
    let res = if r2 = [] then (new_pr, r2) else program r2 new_pr in res
  | Type :: _ ->
    let inst, r1 = binding src in
    let r2 = expect DoubleSemicolon r1 in
    let new_pr = List.append pr [inst] in 
    let res = if r2 = [] then (new_pr, r2) else program r2 new_pr in res 
  | _ -> raise (ParseError ("Unexpected program structure: " ^ (String.concat " " (List.map tok_to_str src))))
  
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
