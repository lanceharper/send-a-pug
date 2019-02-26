open BsReactNative;

let styles =
  StyleSheet.create(
    Style.{
      "heart":
        style([width(Pt(50.)), height(Pt(50.)), position(Absolute)]),
      "heartShape":
        style([
          width(Pt(30.)),
          height(Pt(45.)),
          position(Absolute),
          top(Pt(0.)),
          borderTopLeftRadius(15.),
          borderTopRightRadius(15.),
          backgroundColor(String("slateblue")),
        ]),
      "leftHeart":
        style([Transform.make(~rotate="-45deg", ()), left(Pt(5.))]),
      "rightHeart":
        style([Transform.make(~rotate="45deg", ()), right(Pt(5.))]),
      "textContainer":
        style([
          width(Pt(60.)),
          height(Pt(45.)),
          position(Absolute),
          top(Pt(0.)),
          justifyContent(Center),
          alignItems(Center),
          backgroundColor(String("transparent")),
        ]),
    },
  );

let component = ReasonReact.statelessComponent("Heart");

let make = (~style, ~heartAnimation, ~height, ~clickedBy, _children) => {
  ...component,
  didMount: _self => {
    Animated.start(
      Animated.timing(
        ~value=heartAnimation,
        ~toValue=`raw(height),
        ~duration=3000.0,
        (),
      ),
      (),
    );
  },
  render: _self => {
    <Animated.View style={Style.concat([styles##heart, style])}>
      <View style={Style.concat([styles##heartShape, styles##leftHeart])} />
      <View style={Style.concat([styles##heartShape, styles##rightHeart])} />
      <View style=styles##textContainer>
        <Text> {clickedBy |> ReasonReact.string} </Text>
      </View>
    </Animated.View>;
  },
};