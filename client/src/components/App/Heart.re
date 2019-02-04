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
          borderTopRightRadius(15.),
        ]),
      "filledHeart": style([backgroundColor(String("#e31745"))]),
      "fit": style([Transform.make(~scaleX=0.9, ())]),
      "emptyFill": style([backgroundColor(String("white"))]),
      "empty": style([backgroundColor(String("#ccc"))]),
      "leftHeart":
        style([Transform.make(~rotate="-45deg", ()), left(Pt(5.))]),
      "rightHeart":
        style([Transform.make(~rotate="45deg", ()), right(Pt(5.))]),
    },
  );

let component = ReasonReact.statelessComponent("Trigger");

let make = (~filled, ~style, _children) => {
  ...component,
  render: _self => {
    let centerNonFilled =
      <View style={Style.concat([StyleSheet.absoluteFill, styles##fit])}>
        <View
          style={Style.concat([
            styles##leftHeart,
            styles##heartShape,
            styles##emptyFill,
          ])}
        />
        <View
          style={Style.concat([
            styles##rightHeart,
            styles##heartShape,
            styles##emptyFill,
          ])}
        />
      </View>;
    let fillStyle = filled ? styles##filledHeart : styles##empty;

    <Animated.View style={Style.concat([styles##heart, style])}>
      <View
        style={Style.concat([
          styles##leftHeart,
          styles##heartShape,
          fillStyle,
        ])}
      />
      <View
        style={Style.concat([
          styles##rightHeart,
          styles##heartShape,
          fillStyle,
        ])}
      />
      {filled ? ReasonReact.null : centerNonFilled}
    </Animated.View>;
  },
};