open BsReactNative;

let styles =
  StyleSheet.create(
    Style.{
      "heartContainer":
        style([
          width(Pct(100.)),
          height(Pt(200.)),
          backgroundColor(String("transparent")),
          position(Absolute),
        ]),
    },
  );

let component = ReasonReact.statelessComponent("HeartContainer");

let make = children => {
  ...component,
  render: _self =>
    <View
      style={
        styles##heartContainer;
      }> ...children </View>,
};