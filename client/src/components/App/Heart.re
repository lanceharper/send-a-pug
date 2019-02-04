open BsReactNative;

let styles =
  StyleSheet.create(
    Style.{
      "heart":
        style([
          width(Pt(50.)),
          height(Pt(50.)),
          backgroundColor(String("transparent")),
        ]),
      "heartShape":
        style([
          width(Pt(30.)),
          height(Pt(45.)),
          position(Absolute),
          top(Pt(0.)),
          borderTopLeftRadius(15.),
          borderTopLeftRadius(15.),
        ]),
      "filledHeart": style([backgroundColor(String("#e31745"))]),
      "fit": style([Transform.make(~scaleX=0.9, ())]),
    },
  );

let component = ReasonReact.statelessComponent("Trigger");

let make = (~onPress, _children) => {
  ...component,
  render: _self =>
    <TouchableWithoutFeedback onPress={_self => onPress({clicked: true})}>
      <View style=containerStyle>
        <Text> {ReasonReact.string("clap!")} </Text>
      </View>
    </TouchableWithoutFeedback>,
};