open BsReactNative;

let containerStyle =
  Style.(
    style([
      height(Pt(30.)),
      width(Pct(100.)),
      backgroundColor(String("transparent")),
      justifyContent(Center),
      alignItems(FlexEnd),
    ])
  );

let headerText =
  Style.(
    style([
      fontFamily("Lilita One"),
      color(String("white")),
      fontSize(Float(20.0)),
    ])
  );

let component = ReasonReact.statelessComponent("AppHeader");

let make = _children => {
  ...component,
  render: _self =>
    <View style=containerStyle>
      <Text style=headerText> {"SEND A PUG" |> ReasonReact.string} </Text>
    </View>,
};