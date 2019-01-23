/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;
let styles =
  StyleSheet.create(
    Style.(
      {
        "navBar":
          style([
            flexDirection(Row),
            backgroundColor(String("#f1f1f1")),
            paddingVertical(Pt(8.)),
            paddingHorizontal(Pt(16.)),
          ]),
        "link":
          style([
            paddingVertical(Pt(4.)),
            paddingHorizontal(Pt(8.)),
            marginRight(Pt(8.)),
          ]),
      }
    ),
  );

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  didMount: _ =>
    onAuthorized(twitchAuth => Js.log(clientIdGet(twitchAuth))),
  render: _self =>
    <View>
      <View style=styles##navBar>
        <View style=styles##link>
          <Text> (ReasonReact.string("Home :|")) </Text>
        </View>
      </View>
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, jsProps => make([||]));