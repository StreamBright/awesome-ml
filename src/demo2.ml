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

(* OCaml supports new data types as easy as the following for 2D points Euclidean space *)
type point = {x:float; y:float;}

(* with the new type we can define the origin easily*)
let origin =  {x=0.0; y=0.0}

(*  Euclidean distance between two 2D points https://en.wikipedia.org/wiki/Euclidean_distance#Two_dimensions
 *
 *  Interestingly using the x ** 2.0 form of power of two is much slower than x .* x (to be investigated)
 *
 *)
let distance_between_points p q =
  (q.x -. p.x) *. (q.x -. p.x)  +. (q.y -. p.y) *. (q.y -. p.y)
  |> sqrt

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
