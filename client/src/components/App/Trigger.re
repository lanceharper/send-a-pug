open BsReactNative;

type clickable = {clicked: bool};
let containerStyle =
  Style.(
    style([
      borderRadius(20.),
      height(Pt(40.)),
      width(Pt(40.)),
      marginBottom(Pt(80.)),
      marginLeft(Pt(20.)),
      backgroundColor(String("slateblue")),
      justifyContent(Center),
      alignItems(Center),
    ])
  );

let component = ReasonReact.statelessComponent("Trigger");

let make = (~onPress, _children) => {
  ...component,
  render: _self =>
    <TouchableWithoutFeedback onPress={_self => onPress(true)}>
      <View style=containerStyle />
    </TouchableWithoutFeedback>,
};