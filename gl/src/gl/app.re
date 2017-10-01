module Constants = RGLConstants;
module Gl = Reasongl.Gl;

type glCamera = {projectionMatrix: Gl.Mat4.t};

/*is this used?*/
type glEnv = {camera: glCamera, window: Gl.Window.t, context: Gl.contextT};
/*Describes a SQUare in GL land*/
type glSquare = {x: int, y: int, height: int, width: int};

let getSquareVertices glSquare::(glSqr: glSquare) => [|
    /* top right point */
    float_of_int @@ glSqr.x + glSqr.width,  float_of_int @@ glSqr.y + glSqr.height,   0.0,
    /* top left point */
    float_of_int glSqr.x,                   float_of_int @@ glSqr.y + glSqr.height,   0.0,
    /* bottom right point */
    float_of_int @@ glSqr.x + glSqr.width,  float_of_int glSqr.y,                     0.0,
    /* bottom left point */
    float_of_int glSqr.x,                   float_of_int glSqr.y,                     0.0
  |];

let randomFloatOne () =>
  Random.float 1.0;

let getSquareColors () => [|
    randomFloatOne(), randomFloatOne(), randomFloatOne(), randomFloatOne(),
    randomFloatOne(), randomFloatOne(), randomFloatOne(), randomFloatOne(),
    randomFloatOne(), randomFloatOne(), randomFloatOne(), randomFloatOne(),
    randomFloatOne(), randomFloatOne(), randomFloatOne(), randomFloatOne()
|];

let getCompiledCorrectly context::context shader::shader =>
  Gl.getShaderParameter context::context shader::shader paramName::Gl.Compile_status == 1;

let compileShaders context::context shader::shader source::source => {
  Gl.shaderSource context::context shader::shader source::source;
  Gl.compileShader context::context shader;
  getCompiledCorrectly context::context shader::shader;
};

let linkProg ::context ::program ::vertexShader ::fragmentShader => {
  Gl.attachShader ::context ::program shader::vertexShader;
  Gl.deleteShader ::context vertexShader;
  Gl.attachShader ::context ::program shader::fragmentShader;
  Gl.deleteShader ::context fragmentShader;
  Gl.linkProgram ::context program;
  Gl.getProgramParameter ::context ::program paramName::Gl.Link_status == 1;
};

let getProgram
  context::(context: Gl.contextT) vertexShader::(vertexShaderSource: string)
  fragmentShader::(fragmentShaderSource: string) :option Gl.programT => {
    Random.init 314;
    /* Starting with vertex shader */
    let vertexShader = Gl.createShader ::context Constants.vertex_shader;
    let compiledCorrectlyVertex = compileShaders context::context shader::vertexShader source::vertexShaderSource;
    if compiledCorrectlyVertex {
      /* Next is fragment shader */
      let fragmentShader = Gl.createShader ::context Constants.fragment_shader;
      let compiledCorrectlyFragment = compileShaders context::context shader::fragmentShader source::fragmentShaderSource;
      if compiledCorrectlyFragment {
        /*Finally linking all together*/
        let program = Gl.createProgram ::context;
        let linkedCorrectly = linkProg ::context ::program ::vertexShader ::fragmentShader;
        if linkedCorrectly {
          /*All good*/
          Some program
        } else {
          print_endline @@ "Linking error: " ^ Gl.getProgramInfoLog ::context program;
          None
        }
      } else {
        print_endline @@ "Fragment shader error: " ^ Gl.getShaderInfoLog ::context fragmentShader;
        None
      }
    } else {
      print_endline @@ "Vertex shader error: " ^ Gl.getShaderInfoLog ::context vertexShader;
      None
    }
};

/**
 * Dumb vertex shader which take for input a vertex position and a vertex color and maps the point onto
 * the screen.
 * Fragment shader simply applies the color to the pixel.
 */
let vertexShaderSource = {|
  attribute vec3 aVertexPosition;
  attribute vec4 aVertexColor;
  uniform mat4 uPMatrix;
  varying vec4 vColor;
  void main(void) {
    gl_Position = uPMatrix * vec4(aVertexPosition, 1.0);
    vColor = aVertexColor;
  }
|};

let fragmentShaderSource = {|
  varying vec4 vColor;
  void main(void) {
    gl_FragColor = vColor;
  }
|};


/** This initializes the window **/
let window = Gl.Window.init argv::Sys.argv;
let windowSize = 800;

Gl.Window.setWindowSize ::window width::windowSize height::windowSize;

/** Initialize the Gl context **/
let context = Gl.Window.getContext window;

Gl.viewport ::context x::0 y::0 width::windowSize height::windowSize;

/* Gl.clearColor context 1.0 1.0 1.0 1.0; */
Gl.clear ::context mask::(Constants.color_buffer_bit lor Constants.depth_buffer_bit);

let camera = {projectionMatrix: Gl.Mat4.create ()};

let vertexBuffer = Gl.createBuffer ::context;
let colorBuffer = Gl.createBuffer ::context;

/** Compiles the shaders and gets the program with the shaders loaded into **/
let program =
  switch (getProgram ::context vertexShader::vertexShaderSource fragmentShader::fragmentShaderSource) {
    | None => failwith "Could not create the program and/or the shaders. Aborting."
    | Some program => program
  };

Gl.useProgram ::context program;

/** Get the attribs ahead of time to be used inside the render function **/
let aVertexPosition = Gl.getAttribLocation ::context ::program name::"aVertexPosition";
Gl.enableVertexAttribArray ::context attribute::aVertexPosition;

let aVertexColor = Gl.getAttribLocation ::context ::program name::"aVertexColor";
Gl.enableVertexAttribArray ::context attribute::aVertexColor;

let pMatrixUniform = Gl.getUniformLocation ::context ::program name::"uPMatrix";
Gl.uniformMatrix4fv ::context location::pMatrixUniform value::camera.projectionMatrix;

/**
 * Will mutate the projectionMatrix to be an ortho matrix with the given boundaries.
 * See this link for quick explanation of what this is.
 * https://shearer12345.github.io/graphics/assets/projectionPerspectiveVSOrthographic.png
 */
Gl.Mat4.ortho
  out::camera.projectionMatrix
  left::0.
  right::(float_of_int (Gl.Window.getWidth window))
  bottom::0.
  top::(float_of_int (Gl.Window.getHeight window))
  near::0.
  far::100.;


let render _ => {
  /* 0,0 is the bottom left corner */
  let glSqr = {x: (Random.int 800), y: (Random.int 800), height: 50, width: 50 };
  let square_vertices = getSquareVertices glSquare::glSqr;

  Gl.bindBuffer ::context target::Constants.array_buffer buffer::vertexBuffer;
  Gl.bufferData ::context target::Constants.array_buffer data::Gl.Bigarray.(of_array Float32 square_vertices)
                usage::Constants.static_draw;
  Gl.vertexAttribPointer ::context attribute::aVertexPosition size::3 type_::Constants.float_
                          normalize::false stride::0 offset::0;

  let square_colors = getSquareColors();

  Gl.bindBuffer ::context target::Constants.array_buffer buffer::colorBuffer;
  Gl.bufferData ::context target::Constants.array_buffer data::Gl.Bigarray.(of_array Float32 square_colors)
                usage::Constants.static_draw;
  Gl.vertexAttribPointer ::context attribute::aVertexColor size::4 type_::Constants.float_
                          normalize::false stride::0 offset::0;
  Gl.uniformMatrix4fv ::context location::pMatrixUniform value::camera.projectionMatrix;

  Gl.drawArrays ::context mode::Constants.triangle_strip first::0 count::4
};


/** Start the render loop. **/
Gl.render ::window displayFunc::render ();

