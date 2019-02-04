open BsReactNative;

type clickable = {clicked: bool};
let containerStyle =
  Style.(
    style([
      borderRadius(40.),
      height(Pt(80.)),
      width(Pt(80.)),
      backgroundColor(String("white")),
      justifyContent(Center),
      alignItems(Center),
    ])
  );

let component = ReasonReact.statelessComponent("Trigger");

let make = (~onPress, _children) => {
  ...component,
  render: _self =>
    <TouchableWithoutFeedback onPress={_self => onPress(true)}>
      <View style=containerStyle>
        <Heart filled=true style=Style.(style([])) />
      </View>
    </TouchableWithoutFeedback>,
};