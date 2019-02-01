/* App is the actual entry point for the application. It matches on the route prop to determine which
   sub-component to render */
open BsReactNative;
open Twitch;

let styles =
  StyleSheet.create(
    Style.{
      "container": style([backgroundColor(String("silver")), flex(1.0)]),
      "spacer":
        style([height(Pt(450.)), backgroundColor(String("orange"))]),
      "navBar":
        style([
          flexDirection(Row),
          backgroundColor(String("slateblue")),
          paddingVertical(Pct(2.)),
          paddingHorizontal(Pct(16.)),
        ]),
      "link":
        style([
          paddingVertical(Pt(4.)),
          paddingHorizontal(Pt(8.)),
          marginRight(Pt(8.)),
        ]),
      "headerText":
        style([
          color(String("white")),
          fontSize(Float(19.)),
          fontWeight(`_500),
          fontFamily("Futura"),
        ]),
      "scrollContainer":
        style([backgroundColor(String("white")), height(Pt(250.))]),
    },
  );

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  didMount: _ => onAuthorized(twitchAuth => Js.log(clientIdGet(twitchAuth))),
  render: _self =>
    <View style=styles##container>
      <View style=styles##navBar>
        <View style=styles##link>
          <Text style=styles##headerText>
            {ReasonReact.string("Home? :)")}
          </Text>
        </View>
      </View>
      <ScrollView style=styles##scrollContainer>
        <Wave name="hi" />
        <View style=styles##spacer>
          <Text style=styles##headerText>
            {ReasonReact.string("scrolled")}
          </Text>
        </View>
        <View style=styles##spacer>
          <Text style=styles##headerText>
            {ReasonReact.string("scrolled")}
          </Text>
        </View>
      </ScrollView>
    </View>,
};

let default = ReasonReact.wrapReasonForJs(~component, _ => make([||]));