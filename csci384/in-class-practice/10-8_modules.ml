module type GROUP = sig
  type t
  val times : t -> t -> t
  val one : t 
  val inverse : t -> t
end