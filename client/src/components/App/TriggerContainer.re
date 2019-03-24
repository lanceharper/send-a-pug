open BsReactNative;

let containerStyle =
  Style.(
    style([
      minHeight(Pt(100.)),
      flex(1.0),
      backgroundColor(String("transparent")),
      paddingLeft(Pt(20.)),
      justifyContent(Center),
      alignItems(Center),
    ])
  );

let component = ReasonReact.statelessComponent("TriggerContainer");

let make = (~onPress, _children) => {
  ...component,
  render: _self =>
    <View style=containerStyle>
      <Trigger onPress />
      <Text
        style=Style.(
          style([
            color(String("white")),
            fontFamily("Josefin Sans"),
            fontSize(Float(12.0)),
            paddingTop(Pt(4.)),
          ])
        )>
        {ReasonReact.string("SEND")}
      </Text>
    </View>,
};