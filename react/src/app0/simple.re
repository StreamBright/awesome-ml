open ReasonReact;

type state = {balance: int, click_counter: int};

type action =
  | Increment
  | Decrement;

let inc _event => Increment;
let dec _event => Decrement;

let component = reducerComponent "Index";

let make _children => {
  ...component,
  initialState: fun () => {balance: 0, click_counter: 0},
  reducer: fun action state => (switch action {
      | Increment => Update {balance: state.balance + 1, click_counter: state.click_counter + 1}
      | Decrement => Update {balance: state.balance - 1, click_counter: state.click_counter + 1}
  }),
  render: fun self => {
    <div>
      <button className="button blue" onClick=(self.reduce inc)> (stringToElement " + ") </button>
      <div>(
        "Balance :: "
        ^ (self.state.balance |> string_of_int)
        |> stringToElement
      )
      </div>
      <div>(
        "Click count :: "
        ^ (self.state.click_counter |> string_of_int)
        |> stringToElement
      )
      </div>
      <button className="button blue" onClick=(self.reduce dec)> (stringToElement " - ") </button>
    </div>
  }
};
ReactDOMRe.renderToElementWithId (element @@ make [||]) "index";
