{

type token =
  | Type             (** type - keyword *)
  | Of               (** of - keyword *)
  | Let              (** let - keyword *)
  | Rec              (** rec - keyword *)
  | In               (** in - keyword *)
  | If               (** if - keyword *)
  | Then             (** then - keyword *)
  | Else             (** else - keyword *)
  | Match            (** match - keyword *)
  | With             (** with - keyword *)
  | Fun              (** fun - keyword *)
  | True             (** true - keyword *)
  | False            (** false - keyword *)
  | Mod              (** mod - keyword *)
  | TInt             (** int - type name *)
  | TBool            (** bool - type name *)
  | TString          (** string - type name *)
  | TUnit            (** unit - type name *)
  | Eq               (** = - binary operator *)
  | Plus             (** + - binary operator *)
  | Minus            (** - - binary operator *)
  | Times            (** * - binary operator *)
  | Divide           (** / - binary operator *)
  | Lt               (** < - binary operator *)
  | Concat           (** ^ - binary operator *)
  | And              (** && - binary operator *)
  | Or               (** || - binary operator *)
  | Not              (** ! - unary operator *)
  | Negate           (** ~ - unary operator *)
  | DoubleSemicolon  (** ;; *)
  | Colon            (** : *)
  | Arrow            (** -> *)
  | DoubleArrow      (** => *)
  | LParen           (** ( *)
  | RParen           (** ) *)
  | Pipe             (** | *)
  | Comma            (** , *)
  | Id of string     (** Identifier, like a variable or function name *)
  | Tid of string    (** Type identifier *)
  | Int of int       (** Integer literal *)
  | String of string (** String literal *)
  | EOF              (** End-of-file - you can ignore this *)

let tok_to_str : token -> string = function
  | Type -> "type"
  | Of -> "of"
  | Let -> "let"
  | Rec -> "rec"
  | In -> "in"
  | If -> "if"
  | Then -> "then"
  | Else -> "else"
  | Match -> "match"
  | With -> "with"
  | Fun -> "fun"
  | True -> "true"
  | False -> "false"
  | Mod -> "mod"
  | TInt -> "int"
  | TBool -> "bool"
  | TString -> "string"
  | TUnit -> "unit"
  | Eq -> "="
  | Plus -> "+"
  | Minus -> "-"
  | Times -> "*"
  | Divide -> "/"
  | Lt -> "<"
  | Concat -> "^"
  | And -> "&&"
  | Or -> "||"
  | Not -> "not"
  | Negate -> "~"
  | DoubleSemicolon -> ";;"
  | Colon -> ":"
  | Arrow -> "->"
  | DoubleArrow -> "=>"
  | LParen -> "("
  | RParen -> ")"
  | Pipe -> "|"
  | Comma -> ","
  | Id id -> id
  | Tid id -> "'" ^ id
  | Int i -> string_of_int i
  | String s -> "\"" ^ s ^ "\""
  | EOF -> "<eof>"

(** A (stateful) buffer for storing string constants. *)
let sb = Buffer.create 256

(** A new kind of error to be thrown when lexing fails. *)
exception SyntaxError of string

}

(** Identifiers start with a letter or underscore then have any number of
    letters, underscores, or digits. *)
let id = ['_' 'a'-'z' 'A'-'Z'] ['_' 'a'-'z' 'A'-'Z' '0'-'9']*
(** A type identifier is exactly like an identifier except that it starts
    with a single quote character. *)
let tid = '\'' ['_' 'a'-'z' 'A'-'Z'] ['_' 'a'-'z' 'A'-'Z' '0'-'9']*

let whitespace = [' ' '\t' '\r' '\n']+
let int = ['0'-'9']+

rule tok = parse
| whitespace
    { tok lexbuf }
| int as i
    { Int (int_of_string i) }
| "type"
    { Type }
| "of"
    { Of }
| "let"
    { Let }
| "rec"
    { Rec}
| "in"
    { In }
| "if"
    { If }
| "then"
    { Then }
| "else"
    { Else }
| "match"
    { Match }
| "with"
    { With }
| "fun"
    { Fun }
| "true"
    { True }
| "false"
    { False }
| "mod"
    { Mod }
| "int"
    { TInt }
| "bool"
    { TBool }
| "string"
    { TString }
| "unit"
    { TUnit }
| '='
    { Eq }
| '+'
    { Plus }
| '-'
    { Minus }
| '*'
    { Times }
| '/'
    { Divide }
| '<'
    { Lt }
| '^'
    { Concat }
| "not"
    { Not }
| '~'
    { Negate }
| "&&"
    { And }
| "||"
    { Or }
| ";;"
    { DoubleSemicolon }
| ':'
    { Colon }
| "->"
    { Arrow }
| "=>"
    { DoubleArrow }
| '('
    { LParen }
| ')'
    { RParen }
| '|'
    { Pipe }
| ','
    { Comma }
| '"'
    { Buffer.clear sb;
      string lexbuf;
      String (Buffer.contents sb) }
| "(*"
    { comment lexbuf }
| id as s
    { Id s }
| tid as s
    { Tid (String.sub s 1 (String.length s - 1)) }
| eof
    { EOF }
| _
    { raise (SyntaxError ("Unexpected char: " ^ Lexing.lexeme lexbuf)) }
and string = parse
| '"'
    { () }
| _ as c
    { Buffer.add_char sb c;
      string lexbuf }
and comment = parse
| "*)"
    { tok lexbuf }
| _
    { comment lexbuf }

{
let tokenize (s : string) : token list =
  let buf = Lexing.from_string s in
  let rec helper acc =
    match tok buf with
    | EOF -> List.rev acc
    | t -> helper (t :: acc) in
  helper []
}
