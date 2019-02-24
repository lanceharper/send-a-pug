/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;

type authenticationResult = {accessToken: string};

let decodeAuthenticationResult = json =>
  Json.Decode.{accessToken: json |> field("AccessToken", string)};

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

  didMount: self =>
    onAuthorized(twitchAuth =>
      Js.Promise.(
        tokenGet(twitchAuth)
        |> Auth.requestUserPoolToken
        |> then_(json =>
             json
             |> decodeAuthenticationResult
             |> (
               authenticationResult =>
                 self.send(Authenticate(authenticationResult.accessToken))
             )
             |> resolve
           )
        |> ignore
      )
    ),

  initialState: () => None,

  reducer: (action, _state) =>
    switch (action) {
    | Authenticate(token) => ReasonReact.Update(Some(token))
    },
  render: self =>
    <View>
      {switch (self.state) {
       | Some(token) => <Authenticated token />
       | None => ReasonReact.null
       }}
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));