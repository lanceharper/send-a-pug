open BsReactNative;

let containerStyle =
  Style.(
    style([
      borderRadius(40.),
      height(Pt(80.)),
      width(Pt(80.)),
      backgroundColor(String("red")),
      justifyContent(Center),
      alignItems(Center),
    ])
  );

let component = ReasonReact.statelessComponent("Trigger");

let make = (~onPress, _children) => {
  ...component,
  render: _self =>
    <TouchableWithoutFeedback onPress>
      <View style=containerStyle>
        <Text> {ReasonReact.string("clap")} </Text>
      </View>
    </TouchableWithoutFeedback>,
};