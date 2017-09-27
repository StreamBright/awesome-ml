(* Not all OCaml is functional, with ref and set of operators you have access to mutable variables *)

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
    if distance_from_origin p < 1.0 then
      within := !within + 1
  done;
  ((float_of_int !within) /. (float_of_int s)) *. 4.0

let print_results pi_estimated pi_known =
  Printf.sprintf "Pi estimated :: %0.10f Pi known %0.10f Difference :: %0.10f " pi_estimated pi_known (pi_known -. pi_estimated)

let () =
  Js.log (print_results (find_pi 1000000) pi_known)
