module React = ReasonReact;

type state = {balance: int, click_counter: int};

type action =
  | Increment
  | Decrement
  | Reset;

let inc _event => Increment;
let dec _event => Decrement;
let res _event => Reset;

let component = React.reducerComponent "Index";

let make _children => {
  ...component,
  initialState: fun () => {balance: 0, click_counter: 0},
  reducer: fun action state => (switch action {
      | Increment => Update {balance: state.balance + 1, click_counter: state.click_counter + 1}
      | Decrement => Update {balance: state.balance - 1, click_counter: state.click_counter + 1}
      | Reset => Update {balance: 0, click_counter: 0}
  }),
  render: fun self => {
    <div>
      <button onClick=(self.reduce dec)> (React.stringToElement "-") </button>
      <div>
        (
          "Balance :: "
          ^ (self.state.balance |> string_of_int)
          |> React.stringToElement
        )
        (
          " ::: Click count :: "
          ^ (self.state.click_counter |> string_of_int)
          ^ " "
          |> React.stringToElement
        )
        <button onClick=(self.reduce res)> (React.stringToElement "Reset") </button>
      </div>
      <button onClick=(self.reduce inc)> (React.stringToElement "+") </button>
    </div>
  }
};

ReactDOMRe.renderToElementWithId (React.element @@ make [||]) "index";
