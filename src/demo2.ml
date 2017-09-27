(* *  Imperative features
   *
   *  Most of OCaml works without mutable variables but it is possible to
   *  use imperative features like mutable variables when needed.
   *
   *    Mutable variable
   *
   *      !       to fetch the current value
   *      :=      assign new value
   *
   *      example: a := !a + 1
   *
   *
   * *)

(* *
   *  Files, modules and interfaces
   *
   *      Opening modules can be global or local withing a let scope
   *
   *      ```open Str``` imports all the Str interface into the current module.
   *      Files are mapped to module names eg. file_name -> Module_name , external modules
   *      can be added by importing the library at compile time. Accessing module functions
   *      is always possible with the fully qualified name, eg: Random.float 0.0 if the library
   *      was present at build time (not sure about dynamic linking) or it is part of core OCaml.
   *
   *      Error: Unbound module Core
   *
   *      src/pi.ml -> Pi
   *
   *      src/pi.mli, interface for module Pi
   *
   *      example interface for this file:
   *
   *            type point
   *            val pi_known : float
   *            val origin : point
   *            val distance_between_points : point -> point -> float
   *            val distance_from_origin : point -> float
   *            val find_pi : number_of_samples:int -> float
   *
   *
   *  *)


module R = Random

let pi_known = 3.141592653589793238462643383279502884197169399375105820974944592307816406286

type point = {x:float; y:float;}

let origin =  {x=0.0; y=0.0}

let distance_between_points p q =  (p.x -. q.x) *. (p.x -. q.x) +. (p.y -. q.y) *. (p.y -. q.y)

let distance_from_origin p = distance_between_points p origin

(* http://www.eveandersson.com/pi/monte-carlo-circle *)

let find_pi ~number_of_samples:s =
  Random.self_init();
  let within = ref 0 in
  for i = 1 to s do
    let xr = R.float 1.0 in
    let yr = R.float 1.0 in
    let p = {x=xr; y=yr} in
    if distance_from_origin p < 1.0 then within := !within + 1
  done;
  ((float_of_int !within) /. (float_of_int s)) *. 4.0

let print_results pi_estimated pi_known =
  Printf.sprintf "Pi estimated :: %0.10f Pi known %0.10f Difference :: %0.10f " pi_estimated pi_known (pi_known -. pi_estimated)

let () =
  Js.log (print_results (find_pi 1000000) pi_known)
