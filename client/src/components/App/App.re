/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;

type token = string;
type isAuthenticated = bool;

type action =
  | Authenticate(string);

type state =
  | Some(token)
  | None;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,

  didMount: _ =>
    onAuthorized(twitchAuth => {
      Js.log(tokenGet(twitchAuth));
      tokenGet(twitchAuth) |> Auth.requestUserPoolToken |> ignore;

      ();
    }),

  initialState: () => None,

  reducer: (action, _state) =>
    switch (action) {
    | Authenticate(token) => ReasonReact.Update(Some(token))
    },
  render: _self => <Authenticated />,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));