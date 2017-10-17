/* config */

let modPos = 0.9;
let modTime = 0.00001;
let modVel = 0.02;
let modDir = 0.05;

external pi: float = "Math.PI" [@@bs.val];
let tau = pi *. 2.0;

/* externals */

type simplex;

/* https://github.com/jwagner/simplex-noise.js
 *
 * https://reasonml.github.io/guide/language/external
 * external, or "FFI" (foreign function interface), or simply "interop"
 * (for "interoperability") is how Reason communicates with other languages, like C or JavaScript.
 * like [@@bs.module]
 *
 * https://bucklescript.github.io/bucklescript/Manual.html#_binding_to_simple_js_functions_values
 *
 * Binding to global value: bs.val
 * Scoped values: bs.scope
 * Binding to JavaScript constructor: bs.new
 * Binding to a value from a module: bs.module
 * Binding to method: bs.send, bs.send.pipe
 * Binding to dynamic key access/set: bs.set_index, bs.get_index
 * Binding to Getter/Setter: bs.get, bs.set
 * Splice calling convention: bs.splice
 * Special types on external declarations: bs.string, bs.int, bs.ignore, bs.as, bs.unwrap
 */

external newSimplex : unit => simplex = "simplex-noise" [@@bs.module] [@@bs.new];
external noise2D : simplex => float => float => float = "noise2D" [@@bs.send];
external noise3D : simplex => float => float => float => float = "noise3D" [@@bs.send];

let simplex = newSimplex ();

let module Math = {
  let pi = [%bs.raw "Math.PI"];
};

let module Date = {
  external now : unit => float = "Date.now" [@@bs.val];
};

/* main app state */

type windowT = {
  mutable width: int,
  mutable height: int
};

type vecT = {
  mutable x: float,
  mutable y: float
};

type pointT = {
  pos: vecT,
  vel: vecT
};

type stateT = {
  mutable points: array pointT,
  window: windowT,
};

let state : stateT = {
  points: [| |],
  window: {
    width: 0,
    height: 0
  }
};

/* canvas/context setup */

let canvas = Document.createElement "canvas";
let ctx = Canvas.getContext canvas "2d";

Document.appendChild canvas;

let setCanvasSize _ => {
  let width = (Document.getWidth Document.window);
  let height = (Document.getHeight Document.window);

  Document.setWidth canvas width;
  Document.setHeight canvas height;

  state.window.width = width;
  state.window.height = height;
};

Document.addEventListener Document.window "resize" setCanvasSize;
Document.addEventListener Document.window "DOMContentLoaded" setCanvasSize;

/* app code */

let genItems num callback => {
  let emptyArray = Array.make num 0;

  Array.map (fun i => {
    callback i;
  }) emptyArray;
};

state.points = genItems 5000 (fun _ => {
  {
    pos: {
      x: Random.float 1.,
      y: Random.float 1.
    },
    vel: {
      x: 0.,
      y: 0.
    }
  };
});

let drawCircle (ctx, x, y, r) => {
  Canvas.arc ctx x y r 0. (2. *. Math.pi) false;
};

let draw state => {
  let width = float_of_int state.window.width;
  let height = float_of_int state.window.height;

  Canvas.fillStyle ctx "rgba(255, 255, 255, 0.01)";
  Canvas.fillRect ctx 0. 0. width height;

  let r = 1.;

  Canvas.fillStyle ctx "rgba(20, 120, 200, 0.10)";

  Array.iter (fun point => {
    Canvas.beginPath ctx;

    let x = point.pos.x *. width;
    let y = point.pos.y *. height;

    drawCircle (ctx, x, y, r);

    Canvas.fill ctx;
  }) state.points;
};

let angleToVec angle :vecT => {
  x: (sin (angle *. 2. *. Math.pi)),
  y: (cos (angle *. 2. *. Math.pi))
};

let update t state => {
  state.points = Array.map (fun point => {
    let nx = point.pos.x *. modPos;
    let ny = point.pos.y *. modPos;
    let nz = t *. modTime;

    let noise = noise3D simplex nx ny nz;
    let direction = angleToVec noise;

    point.vel.x = point.vel.x +. direction.x *. modDir;
    point.vel.y = point.vel.y +. direction.y *. modDir;

    point.vel.x = point.vel.x *. modVel;
    point.vel.y = point.vel.y *. modVel;

    point.pos.x = point.pos.x +. point.vel.x;
    point.pos.y = point.pos.y +. point.vel.y;

    if (point.pos.x > 1.0 || point.pos.x < 0.0 ||
        point.pos.y > 1.0 || point.pos.y < 0.0) {
      point.pos.x = Random.float 1.;
      point.pos.y = Random.float 1.;
      point.vel.x = 0.;
      point.vel.y = 0.;
    };

    point;
  }) state.points;
};

let start = Date.now ();

let rec loop () => {
  let t = (Date.now ()) -. start;

  update t state;
  draw state;

  Document.requestAnimationFrame loop;
};

loop ();
