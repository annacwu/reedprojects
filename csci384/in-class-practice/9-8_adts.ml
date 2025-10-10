type day = 
  | Monday
  | Tuesday
  | Wednesday
  | Thursday
  | Friday
  | Saturday
  | Sunday

let is_weekday(d: day) : bool =
  match d with 
    | Saturday -> false
    | Sunday -> false
    | _ -> true

type int_option = 
  | Num of int
  | None 

let default(value : int) (opt: int_option) : int = 
  match opt with
  | Num number -> number
  | None -> value

let rec length (lst : 'a list) : int = match lst with
  | [] -> 0
  | head :: tail -> 1 + length tail

let rec index (lst: 'a list) (i: int): 'a = 
  if i = 0 then match lst with 
    | [] -> failwith "Index does not exist"
    | head :: tail -> head
  else match lst with 
    | [] -> failwith "Index does not exist"
    | head :: tail -> index (tail) (i - 1) 

let nth (l: int list) (i: int) : int_option =
  if length l = i then None
  else Num (index l i)

type color = 
  | Rgb of float * float * float
  | Cmyk of float * float * float * float

let toggle_representation (c: color) : color =
  match c with
    | Rgb rep 
    | Cmyk rep