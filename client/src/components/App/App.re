/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;

type authenticationResult = {
  accessToken: string,
  displayName: string,
  channelId: string,
};

let decodeAuthenticationResult = json =>
  Json.Decode.{
    accessToken: json |> field("AccessToken", string),
    displayName: json |> field("displayName", string),
    channelId: json |> field("channelId", string),
  };

type isAuthenticated = bool;

type action =
  | Authenticate(authenticationResult);

type state =
  | Some(authenticationResult)
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
                 self.send(Authenticate(authenticationResult))
             )
             |> resolve
           )
        |> ignore
      )
    ),

  initialState: () => None,

  reducer: (action, _state) =>
    switch (action) {
    | Authenticate(authenticationResult) =>
      ReasonReact.Update(Some(authenticationResult))
    },
  render: self =>
    <View>
      {switch (self.state) {
       | Some(authenticationResult) =>
         <Authenticated
           token={authenticationResult.accessToken}
           displayName={authenticationResult.displayName}
           channelId={authenticationResult.channelId}
         />
       | None => ReasonReact.null
       }}
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));